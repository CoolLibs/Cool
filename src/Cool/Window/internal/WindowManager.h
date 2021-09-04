#pragma once

#include "../Window.h"

namespace Cool {

class WindowManager {
public:
    std::list<Window>& windows() { return _windows; }
    Window&            find(GLFWwindow* glfw_window);

    Window& main_window()
    {
        assert(_main_window != nullptr && "You forgot to create a main window !");
        return *_main_window;
    }

    void set_main_window(Window& window)
    {
        assert(_main_window == nullptr && "You can only have one main window !");
        _main_window = &window;
    }

private:
    std::list<Window> _windows;
    Window*           _main_window = nullptr;
};

} // namespace Cool
