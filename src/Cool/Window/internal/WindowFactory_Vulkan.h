#pragma once

#include "WindowFactory_Base.h"
#include "Window_Vulkan.h"

namespace Cool {

class WindowFactory_Vulkan : public WindowFactory_Base {
public:
    ~WindowFactory_Vulkan();
    Window_Vulkan& make_main_window(const char* name, int width, int height, bool cap_framerate = true);
    Window_Vulkan& make_secondary_window(const char* name, int width, int height, bool cap_framerate = true);

private:
    Window_Vulkan& make_window(const char* name, int width, int height, bool cap_framerate = true);
    void           setup_imgui(Window_Vulkan& window);
};

} // namespace Cool