#pragma once
// #ifdef __COOL_APP_VULKAN
#include "../../Window.h"
// clang-format off
#include <vku/vku_framework.hpp> // must be included before <vku/vku.hpp>
#include <vku/vku.hpp>
// clang-format on

namespace Cool {

class WindowFactory_Vulkan {
public:
    static void initialize();
    WindowFactory_Vulkan();

    Cool::Window make_window(const char* title, int width, int height) const;

    inline vku::Framework& vku_framework() { return _vku_framework; }

private:
    vku::Framework _vku_framework;
};

} // namespace Cool

// #endif