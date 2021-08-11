#pragma once

#include "WindowFactory_Base.h"
#include "Window_Vulkan.h"

namespace Cool {

class WindowFactory_Vulkan {
public:
    ~WindowFactory_Vulkan();
    Window_Vulkan&            make_window(const char* name, int width, int height);
    std::list<Window_Vulkan>& windows() { return _windows; }

private:
    void setup_imgui(Window_Vulkan& window);

private:
    WindowFactory_Base       _base;
    std::list<Window_Vulkan> _windows;
};

} // namespace Cool