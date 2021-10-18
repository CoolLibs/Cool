#pragma once

#if defined(COOL_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

#include <Cool/AppManager/AppManager.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Window/internal/WindowFactory.h>

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
void run(std::vector<WindowConfig> windows_configs, AppManagerConfig app_manager_config = {})
{
    assert(windows_configs.size() >= 1);
    auto window_factory = Cool::WindowFactory{};
    window_factory.make_main_window(windows_configs[0]);
    for (size_t i = 1; i < windows_configs.size(); ++i) {
        window_factory.make_secondary_window(windows_configs[i]);
    }
    {
        auto app         = App{window_factory.window_manager()};
        auto app_manager = Cool::AppManager{window_factory.window_manager(), app, app_manager_config};
        app_manager.run();
#if defined(COOL_VULKAN)
        vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    }
    Cool::shut_down();
}

} // namespace Cool
