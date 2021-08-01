#pragma once
// #ifdef __COOL_APP_VULKAN
#include "../../Window.h"
// clang-format off
#include <vku/vku_framework.hpp> // must be included before <vku/vku.hpp>
#include <vku/vku.hpp>
// clang-format on

struct GLFWwindow;

namespace Cool {

class WindowFactory_Vulkan {
public:
    static void initialize();
    explicit WindowFactory_Vulkan();
    ~WindowFactory_Vulkan();

    Cool::Window& make_window(const char* title, int width, int height);

    inline vku::Framework& vku_framework() { return _vku_framework; }

private:
    void setup_imgui(Window& window);

private:
    vku::Framework            _vku_framework;
    std::vector<Cool::Window> _windows;
};

} // namespace Cool

// #endif