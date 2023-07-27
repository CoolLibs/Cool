#pragma once

#include <exception>
#include <filesystem>
#include "Cool/Path/Path.h"
#include "Cool/View/ViewsManager.h"
#if defined(COOL_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

#include <Cool/AppManager/AppManager.h>
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Serialization/Serialization.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Window/internal/WindowFactory.h>
#include "Cool/Gpu/TextureLibrary.h"
#include "Cool/Gpu/TextureSamplerLibrary.h"
#include "Cool/ImGui/StyleEditor.h"
//
#include <cereal/archives/json.hpp> // Must be included last because the more files it sees, the more it slows down compilation (by A LOT)

namespace Cool {

namespace internal {
class PreviousSessionLoadingFailed_Exception : public std::exception {};
} // namespace internal

/**
 * @brief Shuts down all the Cool systems
 *
 */
inline void shut_down()
{
#if defined(COOL_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    Icons::shut_down();
    TextureLibrary::instance().clear();
    TextureSamplerLibrary::instance().clear();
    FullscreenPipeline::shut_down();
}

inline auto create_autosaver(Cool::AutoSerializer const& auto_serializer) -> std::function<void()>
{
    return [&auto_serializer]() {
        if (!user_settings().autosave_enabled)
            return;

        static auto last_time = std::chrono::steady_clock::now();
        const auto  now       = std::chrono::steady_clock::now();
        if (now - last_time > std::chrono::duration<float>{user_settings().autosave_delay_in_seconds})
        {
            auto_serializer.save();
            last_time = now;
            if (DebugOptions::log_when_autosaving())
            {
                Log::ToUser::info("Autosave", "The application was just saved.");
            }
        }
    };
}

inline void copy_default_user_data_ifn()
{
    try
    {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(Cool::Path::default_user_data()))
        {
            auto const& default_path = entry.path();
            if (!std::filesystem::is_regular_file(default_path))
                continue;

            auto const path = Cool::Path::user_data() / std::filesystem::relative(default_path, Cool::Path::default_user_data());
            if (std::filesystem::exists(path))
                continue;

            try
            {
                std::filesystem::copy_file(default_path, path);
            }
            catch (std::exception const& e)
            {
                Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy {} to {}:\n{}", std::filesystem::weakly_canonical(default_path), std::filesystem::weakly_canonical(path), e.what()));
            }
        }
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy default user data:\n{}", e.what()));
    }
}

template<typename App>
void run(
    std::vector<WindowConfig> const& windows_configs,
    AppManagerConfig                 app_manager_config = {}
)
{
    // Make sure user_data() folder is populated with all the default_user_data() files.
    copy_default_user_data_ifn();
    // Create window.s
    assert(!windows_configs.empty());
    auto window_factory = Cool::WindowFactory{};
    window_factory.make_main_window(windows_configs[0]);
    for (size_t i = 1; i < windows_configs.size(); ++i)
        window_factory.make_secondary_window(windows_configs[i]);

    {
        Cool::StyleEditor style_autoserializer{};

        // Auto serialize the UserSettings // Done after the creation of the windows because we need an ImGui context to set the color theme
        auto auto_serializer_user_settings = Cool::AutoSerializer{};
        auto_serializer_user_settings.init<UserSettings, cereal::JSONInputArchive>(
            Cool::Path::user_data() / "user-settings.json",
            user_settings(),
            [](OptionalErrorMessage const& error) {
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
                Serialization::save<UserSettings, cereal::JSONOutputArchive>(user_settings(), path, "User Settings");
            }
        );
        user_settings().apply_multi_viewport_setting();

        // Make sure the MessageConsole won't deadlock at startup when the "Log when creating textures" option is enabled (because displaying the console requires the close_button, which will generate a log when its texture gets created).
        Icons::close_button();

        // Create and run the App
        const auto run_loop = [&](bool load_from_file) {
            auto views = ViewsManager{};
            auto app   = App{window_factory.window_manager(), views};
            // Auto serialize the App
            Cool::AutoSerializer auto_serializer;
            auto_serializer.init<App, cereal::JSONInputArchive>(
                Cool::Path::user_data() / "last-session.json", app,
                [](OptionalErrorMessage const& error) {
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
                    Serialization::save<App, cereal::JSONOutputArchive>(app, path, "App");
                },
                load_from_file
            );
            // Run the app
            auto app_manager = Cool::AppManager{window_factory.window_manager(), views, app, app_manager_config};
            app_manager.run(create_autosaver(auto_serializer));
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
    Cool::shut_down();
}

} // namespace Cool
