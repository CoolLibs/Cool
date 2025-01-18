#include "run.h"
#include <Cool/AppManager/AppManager.h>
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Window/internal/WindowFactory.h>
#include <exception>
#include <filesystem>
#include <memory>
#include <scope_guard/scope_guard.hpp>
#include "Audio/Audio.hpp"
#include "Cool/AppManager/IApp.h"
#include "Cool/AppManager/internal/get_app_manager.hpp"
#include "Cool/CommandLineArgs/CommandLineArgs.h"
#include "Cool/Core/set_utf8_locale.hpp"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/File/File.h"
#include "Cool/ImGui/StyleEditor.h"
#include "Cool/Path/Path.h"
#include "Cool/TextureSource/TextureLibrary_Webcam.hpp"
#include "Cool/View/ViewsManager.h"
#include "ImGuiNotify/ImGuiNotify.hpp"
#if defined(COOL_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

namespace Cool::internal {

void shut_down()
{
#if defined(COOL_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    Audio::shut_down();
    TextureLibrary_Webcam::instance().shut_down(); // We must destroy the textures in the WebcamImages before the texture_pool() gets destroyed
}

auto create_autosaver(std::function<void()> const& save) -> std::function<void()>
{
    // This function will be run every frame
    return [save]() {
        if (!user_settings().autosave_enabled)
            return;

        static auto last_time = std::chrono::steady_clock::now();
        const auto  now       = std::chrono::steady_clock::now();
        if (now - last_time > std::chrono::duration<float>{user_settings().autosave_delay_in_seconds})
        {
            save();
            last_time = now;
            if (DebugOptions::log_when_autosaving())
                Log::ToUser::info("Autosave", "The application was just saved.");
        }
    };
}

void copy_default_user_data_ifn()
{
    try
    {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(Cool::Path::default_user_data()))
        {
            auto const& default_path = entry.path();
            if (!Cool::File::is_regular_file(default_path))
                continue;

            auto const path = Cool::Path::user_data() / Cool::File::relative(default_path, Cool::Path::default_user_data());
            if (Cool::File::exists(path))
                continue;

            try
            {
                Cool::File::copy_file(default_path, path);
            }
            catch (std::exception const& e)
            {
                Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy {} to {}:\n{}", Cool::File::weakly_canonical(default_path), Cool::File::weakly_canonical(path), e.what()));
            }
        }
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy default user data:\n{}", e.what()));
    }
}

void run_impl(
    int argc, char** argv, RunConfig const& config,
    std::function<void()> const&                                                                         initialize_paths_config,
    std::function<std::unique_ptr<IApp>(Cool::WindowManager& windows, Cool::ViewsManager& views)> const& make_unique_app,
    std::function<OptionalErrorMessage(IApp&)> const&                                                    load_app,
    std::function<void(IApp const&)> const&                                                              save_app
)
{
    set_utf8_locale();
    command_line_args().init(argc, argv);
    initialize_paths_config();
    bool const ignore_invalid_user_data_file = !Cool::File::exists(Cool::Path::user_data()); // If user_data() does not exist, it means this is the first time you open Coollab, so it is expected that the files will be invalid. Any other time than that, we want to warn because this means that serialization has been broken, which we want to avoid on the dev's side.
    // Make sure user_data() folder is populated with all the default_user_data() files.
    internal::copy_default_user_data_ifn();
    // Create window.s
    assert(!config.windows_configs.empty());
    auto window_factory = Cool::WindowFactory{};
    window_factory.make_main_window(config.windows_configs[0]);
    for (size_t i = 1; i < config.windows_configs.size(); ++i)
        window_factory.make_secondary_window(config.windows_configs[i]);

    {
        Cool::StyleEditor style_autoserializer{};

        // Auto serialize the UserSettings // Done after the creation of the windows because we need an ImGui context to set the color theme
        auto auto_serializer_user_settings = Cool::AutoSerializer{};
        auto_serializer_user_settings.init<UserSettings, ser20::JSONInputArchive>(
            Cool::Path::user_data() / "user-settings.json",
            user_settings(),
            [&](OptionalErrorMessage const& error) {
                if (ignore_invalid_user_data_file)
                    return;
                error.send_error_if_any(
                    [&](std::string const& message) {
                        return Cool::Message{
                            .category = "Loading User Settings",
                            .message  = message,
                            .severity = Cool::MessageSeverity::Warning,
                        };
                    },
                    Cool::Log::ToUser::console()
                );
            },
            [&](std::filesystem::path const& path) {
                Serialization::save<UserSettings, ser20::JSONOutputArchive>(user_settings(), path, "User Settings");
            }
        );
        user_settings().apply_multi_viewport_setting();

        Icons::close_button(); // Make sure the MessageConsole won't deadlock at startup when the "Log when creating textures" option is enabled (because displaying the console requires the close_button, which will generate a log when its texture gets created).

        // Init error callbacks
        Audio::set_error_callback([](RtAudioErrorType /* type */, std::string const& error_message) {
            if (DebugOptions::log_debug_warnings())
                Cool::Log::ToUser::warning("Audio", error_message);
        });

        // Create the App
        auto views = ViewsManager{};
        auto app   = make_unique_app(window_factory.window_manager(), views); // Stored in a unique_ptr because we don't require App to be copy-assignable

        auto const error = load_app(*app);
        if (error)
        {
            app = make_unique_app(window_factory.window_manager(), views); // Make sure to start with a clean default App if deserialization fails, otherwise some fields would have been deserialized and others have their default values, and there could be a mismatch
            if (!ignore_invalid_user_data_file)
            {
                ImGuiNotify::send({
                    .type     = ImGuiNotify::Type::Warning,
                    .title    = "Failed to restore last session's state",
                    .content  = *error.error_message(),
                    .duration = 15s,
                });
            }
        }
        auto const save_on_exit = sg::make_scope_guard([&]() {
            save_app(*app);
        });
        // Run the app
        auto app_manager            = Cool::AppManager{window_factory.window_manager(), views, *app, config.app_manager_config};
        internal::get_app_manager() = &app_manager;
        app_manager.run(internal::create_autosaver([&]() {
            save_app(*app);
        }));
        internal::get_app_manager() = nullptr;
        app->on_shutdown();
#if defined(COOL_VULKAN)
        vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    }
    Cool::internal::shut_down();
}

} // namespace Cool::internal