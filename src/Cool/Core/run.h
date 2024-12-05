#pragma once
#include <Cool/AppManager/AppManager.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Window/internal/WindowFactory.h>
#include "Audio/Audio.hpp"
#include "Cool/Core/set_utf8_locale.hpp"
#include "Cool/ImGui/StyleEditor.h"
#include "hide_console_in_release.h"

#if defined(COOL_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

//
#include <ser20/archives/json.hpp> // Must be included last because the more files it sees, the more it slows down compilation (by A LOT)

namespace Cool {

namespace internal {
class PreviousSessionLoadingFailed_Exception : public std::exception {};
void shut_down();
auto create_autosaver(Cool::AutoSerializer const& auto_serializer) -> std::function<void()>;
void copy_default_user_data_ifn(int imgui_ini_version);
} // namespace internal

struct RunConfig {
    std::vector<WindowConfig> windows_configs{};
    AppManagerConfig          app_manager_config{};
    int                       imgui_ini_version{};
};

template<typename App>
void run(RunConfig const& config)
{
    set_utf8_locale();
    bool const ignore_invalid_user_data_file = !Cool::File::exists(Cool::Path::user_data()); // If user_data() does not exist, it means this is the first time you open Coollab, so it is expected that the files will be invalid. Any other time than that, we want to warn because this means that serialization has been broken, which we want to avoid on the dev's side.
    // Make sure user_data() folder is populated with all the default_user_data() files.
    internal::copy_default_user_data_ifn(config.imgui_ini_version);
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

        // Make sure the MessageConsole won't deadlock at startup when the "Log when creating textures" option is enabled (because displaying the console requires the close_button, which will generate a log when its texture gets created).
        Icons::close_button();

        // Init error callbacks
        Audio::set_error_callback([](RtAudioErrorType /* type */, std::string const& error_message) {
            Cool::Log::ToUser::warning("Audio", error_message);
        });

        // Create and run the App
        const auto run_loop = [&](bool load_from_file) {
            auto views = ViewsManager{};
            auto app   = App{window_factory.window_manager(), views};
            // Auto serialize the App
            Cool::AutoSerializer auto_serializer;
            auto_serializer.init<App, ser20::JSONInputArchive>(
                Cool::Path::user_data() / "last-session.json", app,
                [&](OptionalErrorMessage const& error) {
                    if (ignore_invalid_user_data_file)
                        return;
                    error.send_error_if_any(
                        [&](std::string const& message) {
                            return Cool::Message{
                                .category = "Loading App",
                                .message  = message,
                                .severity = Cool::MessageSeverity::Warning,
                            };
                        },
                        Cool::Log::ToUser::console()
                    );
                    throw internal::PreviousSessionLoadingFailed_Exception{}; // Make sure to start with a clean default App if deserialization fails
                },
                [&](std::filesystem::path const& path) {
                    Serialization::save<App, ser20::JSONOutputArchive>(app, path, "App");
                },
                load_from_file
            );
            // Run the app
            auto app_manager = Cool::AppManager{window_factory.window_manager(), views, app, config.app_manager_config};
            app_manager.run(internal::create_autosaver(auto_serializer));
            app.on_shutdown();
#if defined(COOL_VULKAN)
            vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
        };
        try
        {
            run_loop(true);
        }
        catch (const internal::PreviousSessionLoadingFailed_Exception&) // Make sure to start with a clean default App if deserialization fails
        {
            run_loop(false);
        }
    }
    Cool::internal::shut_down();
}

} // namespace Cool
