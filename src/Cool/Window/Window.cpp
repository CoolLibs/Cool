#include "Window.h"

namespace Cool {

Window::Window(GLFWwindow* glfw_window)
    : _glfw_window{glfw_window}
{
}

bool Window::check_for_fullscreen_toggles(int key, int /*scancode*/, int action, int /*mods*/)
{
    if (action == GLFW_RELEASE)
    {
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
    }
    return false;
}

void Window::toggle_fullscreen()
{
    if (_is_fullscreen)
    {
        escape_fullscreen();
    }
    else
    {
        GLFWmonitor*       monitor = current_monitor();
        const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
        glfwGetWindowPos(*_glfw_window, &_pos_x_before_fullscreen, &_pos_y_before_fullscreen);
        glfwGetWindowSize(*_glfw_window, &_width_before_fullscreen, &_height_before_fullscreen);
        glfwSetWindowMonitor(*_glfw_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
#if defined(COOL_OPENGL)
        cap_framerate_if(framerate_is_capped()); // Turning fullscreen disables VSync so we have to reenable it
#endif
        _is_fullscreen = true;
    }
}

void Window::escape_fullscreen()
{
    if (_is_fullscreen)
    {
        glfwSetWindowMonitor(*_glfw_window, nullptr, _pos_x_before_fullscreen, _pos_y_before_fullscreen, _width_before_fullscreen, _height_before_fullscreen, 0);
        _is_fullscreen = false;
    }
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

bool Window::imgui_cap_framerate()
{
    bool should_cap_framerate = framerate_is_capped();
    bool checkbox_triggered   = ImGui::Checkbox("Capped Framerate", &should_cap_framerate);
    if (checkbox_triggered)
    {
        cap_framerate_if(should_cap_framerate);
    }
    return checkbox_triggered;
}

void Window::cap_framerate_if(bool should_cap)
{
    return _impl.cap_framerate_if(should_cap, *_glfw_window);
}

[[nodiscard]] auto Window::framerate_is_capped() const -> bool
{
    return _impl.framerate_is_capped();
}

GLFWmonitor* Window::current_monitor() const
{
    // Thanks to https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
    int                nmonitors, i;
    int                wx, wy, ww, wh;
    int                mx, my, mw, mh;
    int                overlap, bestoverlap;
    GLFWmonitor*       bestmonitor;
    GLFWmonitor**      monitors;
    const GLFWvidmode* mode;

    bestoverlap = 0;
    bestmonitor = NULL;

    glfwGetWindowPos(*_glfw_window, &wx, &wy);
    glfwGetWindowSize(*_glfw_window, &ww, &wh);
    monitors = glfwGetMonitors(&nmonitors);

    for (i = 0; i < nmonitors; i++)
    {
        mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap = std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx))
                  * std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (bestoverlap < overlap)
        {
            bestoverlap = overlap;
            bestmonitor = monitors[i];
        }
    }

    return bestmonitor;
}

} // namespace Cool