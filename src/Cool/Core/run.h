#pragma once

#if defined(COOL_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

#include <Cool/AppManager/AppManager.h>
#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Serialization/as_json.h>
#include <Cool/UserSettings/UserSettings.h>
#include <Cool/Window/internal/WindowFactory.h>
#include "Cool/Gpu/TextureLibrary.h"
#include "Cool/Gpu/TextureSamplerLibrary.h"
#include "InitConfig.h"
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

template<typename App>
void run(
    const std::vector<WindowConfig>& windows_configs,
    InitConfig                       init_config        = {},
    AppManagerConfig                 app_manager_config = {}
)
{
    // Init
    init_config.set_paths();
    // Create window.s
    assert(!windows_configs.empty());
    auto window_factory = Cool::WindowFactory{};
    window_factory.make_main_window(windows_configs[0]);
    for (size_t i = 1; i < windows_configs.size(); ++i)
        window_factory.make_secondary_window(windows_configs[i]);

    // Auto serialize the UserSettings // Done after the creation of the windows because we need an ImGui context to set the color theme
    auto auto_serializer_user_settings = Cool::AutoSerializer{};
    auto_serializer_user_settings.init<UserSettings, cereal::JSONInputArchive>(
        Cool::Path::root() / "cache/user-settings.json",
        user_settings(),
        [](OptionalErrorMessage const& error) {
            error.send_error_if_any(
                [&](std::string const& message) {
                    return Cool::Message{
                        .category = "Loading Project",
                        .message  = message,
                        .severity = Cool::MessageSeverity::Warning,
                    };
                },
                Cool::Log::ToUser::console()
            );
        },
        [&](std::filesystem::path const& path) {
            Serialization::to_json<UserSettings, cereal::JSONOutputArchive>(user_settings(), path, "User Settings");
        }
    );

    // Make sure the MessageConsole won't deadlock at startup when the "Log when creating textures" option is enabled (because displaying the console requires the close_button, which will generate a log when its texture gets created).
    Icons::close_button();

    // Create and run the App
    const auto run_loop = [&](bool load_from_file) {
        auto app = App{window_factory.window_manager()};
        // Auto serialize the App
        Cool::AutoSerializer auto_serializer;
        auto_serializer.init<App, cereal::JSONInputArchive>(
            Cool::Path::root() / "cache/last-session.json", app,
            [](OptionalErrorMessage const& error) {
                error.send_error_if_any(
                    [&](std::string const& message) {
                        return Cool::Message{
                            .category = "Loading Project",
                            .message  = message,
                            .severity = Cool::MessageSeverity::Warning,
                        };
                    },
                    Cool::Log::ToUser::console()
                );
                throw internal::PreviousSessionLoadingFailed_Exception{}; // Make sure to start with a clean default App if deserialization fails
            },
            [&](std::filesystem::path const& path) {
                Serialization::to_json<App, cereal::JSONOutputArchive>(app, path, "App");
            },
            load_from_file
        );
        // Run the app
        auto app_manager = Cool::AppManager{window_factory.window_manager(), app, app_manager_config};
        app_manager.run(create_autosaver(auto_serializer));
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
    Cool::shut_down();
}

} // namespace Cool
