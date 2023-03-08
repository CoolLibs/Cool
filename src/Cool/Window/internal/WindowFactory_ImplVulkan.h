#pragma once
#if defined(COOL_VULKAN)

#include "../WindowManager.h"
#include "WindowConfig.h"
#include "Window_Vulkan.h"

namespace Cool {

class WindowFactory_ImplVulkan {
public:
    WindowFactory_ImplVulkan() = default;
    static void shut_down(WindowManager& window_manager);

    void           setup_main_window(Window_Vulkan& window);
    static void    setup_secondary_window(Window_Vulkan&, WindowManager&);
    Window_Vulkan& make_window(const WindowConfig& config, WindowManager& window_manager);

private:
    void setup_imgui(Window_Vulkan& window);
};

} // namespace Cool

#endif