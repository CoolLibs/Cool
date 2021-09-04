#pragma once

#include "../Window.h"

namespace Cool {

class WindowManager {
public:
    std::list<Window>& windows() { return _windows; }
    void               set_main_window(Window& window) { _main_window = &window; }
    Window&            find(GLFWwindow* glfw_window);
    Window&            main_window()
    {
        assert(_main_window != nullptr);
        return *_main_window;
    }

private:
    std::list<Window> _windows;
    Window*           _main_window;
};

} // namespace Cool
