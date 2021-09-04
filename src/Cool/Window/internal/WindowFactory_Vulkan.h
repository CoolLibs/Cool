#pragma once

#include "WindowCreationParams.h"
#include "WindowManager.h"
#include "Window_Vulkan.h"

namespace Cool {

class WindowFactory_Vulkan {
public:
    void shut_down(WindowManager& window_manager);

    void           setup_main_window(Window_Vulkan& window);
    void           setup_secondary_window(Window_Vulkan& window, WindowManager& window_manager);
    Window_Vulkan& make_window(const WindowCreationParams& params, WindowManager& window_manager);

private:
    void setup_imgui(Window_Vulkan& window);
};

} // namespace Cool