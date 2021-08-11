#pragma once

#include "WindowFactory_Base.h"
#include "WindowManager.h"
#include "Window_Vulkan.h"

namespace Cool {

class WindowFactory_Vulkan {
public:
    ~WindowFactory_Vulkan();
    Window_Vulkan& make_window(const char* name, int width, int height);
    WindowManager& window_manager() { return _window_manager; }

private:
    void setup_imgui(Window_Vulkan& window);

private:
    WindowFactory_Base _base;
    WindowManager      _window_manager;
};

} // namespace Cool