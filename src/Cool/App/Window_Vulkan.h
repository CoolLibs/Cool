#pragma once

#include "Window.h"

namespace Cool {

class Window_Vulkan {
public:
    inline Window& operator*() { return _window; }
    inline Window* operator->() { return &operator*(); }

private:
    Window _window;
};

} // namespace Cool