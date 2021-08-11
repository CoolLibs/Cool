#pragma once

#include "../Window.h"

namespace Cool {

class WindowManager {
public:
    std::list<Window>& windows() { return _windows; }
    Window&            find(GLFWwindow* glfw_window);

private:
    std::list<Window> _windows;
};

} // namespace Cool
