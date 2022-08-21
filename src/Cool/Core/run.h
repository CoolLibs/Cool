#pragma once

#if defined(COOL_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

#include <Cool/AppManager/AppManager.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Window/internal/WindowFactory.h>
#include "InitConfig.h"

namespace Cool {

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
    FullscreenPipeline::shut_down();
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
    assert(windows_configs.size() >= 1);
    auto window_factory = Cool::WindowFactory{};
    window_factory.make_main_window(windows_configs[0]);
    for (size_t i = 1; i < windows_configs.size(); ++i)
    {
        window_factory.make_secondary_window(windows_configs[i]);
    }
    // Create and run the App
    const auto run_loop = [&](bool load_from_file) {
        auto                      app = App{window_factory.window_manager()};
        Cool::AutoSerializer<App> _auto_serializer{Cool::Path::root() + "/cache/last-session.json", "App", app, true, load_from_file};
        auto                      app_manager = Cool::AppManager{window_factory.window_manager(), app, app_manager_config};
        app_manager.run();
#if defined(COOL_VULKAN)
        vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    };
    try
    {
        run_loop(true);
    }
    catch (const Cool::Serialization::Exception&) // Make sure to start with a clean default App if deserialization fails
    {
        run_loop(false);
    }
    Cool::shut_down();
}

} // namespace Cool
