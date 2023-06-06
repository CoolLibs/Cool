#include "Window.h"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

Window::Window(GLFWwindow* glfw_window)
    : _glfw_window{glfw_window}
{
}

auto Window::check_for_fullscreen_toggles(int key, int /*scancode*/, int action, int /*mods*/) -> bool
{
    if (action != GLFW_RELEASE)
        return false;

    if (key == GLFW_KEY_F11)
    {
        toggle_fullscreen();
        return true;
    }
    if (key == GLFW_KEY_ESCAPE && _is_fullscreen)
    {
        escape_fullscreen();
        return true;
    }

    return false;
}

void Window::toggle_fullscreen()
{
    if (_is_fullscreen)
        escape_fullscreen();
    else
        turn_on_fullscreen();
}

void Window::set_fullscreen(bool is_fullscreen)
{
    if (is_fullscreen)
        turn_on_fullscreen();
    else
        escape_fullscreen();
}

void Window::escape_fullscreen()
{
    if (!_is_fullscreen)
        return;

    glfwSetWindowMonitor(*_glfw_window, nullptr, _pos_x_before_fullscreen, _pos_y_before_fullscreen, _width_before_fullscreen, _height_before_fullscreen, 0);
    _is_fullscreen = false;
}

void Window::turn_on_fullscreen()
{
    if (_is_fullscreen)
        return;

    GLFWmonitor*       monitor = current_monitor();
    GLFWvidmode const* mode    = glfwGetVideoMode(monitor);
    glfwGetWindowPos(*_glfw_window, &_pos_x_before_fullscreen, &_pos_y_before_fullscreen);
    glfwGetWindowSize(*_glfw_window, &_width_before_fullscreen, &_height_before_fullscreen);
    glfwSetWindowMonitor(*_glfw_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
#if defined(COOL_OPENGL)
        cap_framerate_if(framerate_is_capped()); // Turning fullscreen disables VSync so we have to reenable it
#endif
        _is_fullscreen = true;
}

void Window::set_visibility(bool is_visible)
{
    _is_visible = is_visible;
    if (is_visible)
    {
        glfwShowWindow(*_glfw_window);
    }
    else
    {
        escape_fullscreen(); // If fullscreen, glfwHideWindow does nothing
        glfwHideWindow(*_glfw_window);
    }
}

auto Window::imgui_cap_framerate() -> bool
{
    bool       should_cap_framerate = framerate_is_capped();
    bool const checkbox_triggered   = ImGuiExtras::toggle("Capped Framerate", &should_cap_framerate);
    if (checkbox_triggered)
        cap_framerate_if(should_cap_framerate);
    return checkbox_triggered;
}

void Window::cap_framerate_if(bool should_cap)
{
    return _impl.cap_framerate_if(should_cap, *_glfw_window);
}

auto Window::framerate_is_capped() const -> bool
{
    return _impl.framerate_is_capped();
}

auto Window::current_monitor() const -> GLFWmonitor*
{
    // Thanks to https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
    int                nb_monitors, i;        // NOLINT
    int                wx, wy, ww, wh;        // NOLINT
    int                mx, my, mw, mh;        // NOLINT
    int                overlap, best_overlap; // NOLINT
    GLFWmonitor*       best_monitor;          // NOLINT
    GLFWmonitor**      monitors;              // NOLINT
    const GLFWvidmode* mode;                  // NOLINT

    best_overlap = 0;
    best_monitor = nullptr;

    glfwGetWindowPos(*_glfw_window, &wx, &wy);
    glfwGetWindowSize(*_glfw_window, &ww, &wh);
    monitors = glfwGetMonitors(&nb_monitors);

    for (i = 0; i < nb_monitors; i++)
    {
        mode = glfwGetVideoMode(monitors[i]);     // NOLINT
        glfwGetMonitorPos(monitors[i], &mx, &my); // NOLINT
        mw = mode->width;
        mh = mode->height;

        overlap = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx))
                  * std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (best_overlap < overlap)
        {
            best_overlap = overlap;
            best_monitor = monitors[i]; // NOLINT
        }
    }

    return best_monitor;
}

} // namespace Cool