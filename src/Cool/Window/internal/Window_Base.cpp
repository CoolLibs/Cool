#include "Window_Base.h"

namespace Cool {

Window_Base::Window_Base(GLFWwindow* m_window)
    : m_window{m_window}
{
}

Window_Base::Window_Base(Window_Base&& o) noexcept
    : m_window(o.m_window)
{
    o.m_window = nullptr;
}

Window_Base& Window_Base::operator=(Window_Base&& o) noexcept
{
    m_window   = o.m_window;
    o.m_window = nullptr;
    return *this;
}

void Window_Base::destroy()
{
    if (m_window != nullptr) // Could have been moved
        glfwDestroyWindow(m_window);
}

bool Window_Base::checkForFullscreenToggles(int key, int scancode, int action, int mods)
{
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_F11) {
            switchFullScreen();
            return true;
        }
        if (key == GLFW_KEY_ESCAPE && m_bIsFullScreen) {
            escapeFullScreen();
            return true;
        }
    }
    return false;
}

void Window_Base::switchFullScreen()
{
    if (m_bIsFullScreen)
        escapeFullScreen();
    else {
        GLFWmonitor*       monitor = getCurrentMonitor();
        const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
        glfwGetWindowPos(m_window, &m_posXBeforeFullscreen, &m_posYBeforeFullscreen);
        glfwGetWindowSize(m_window, &m_widthBeforeFullscreen, &m_heightBeforeFullscreen);
        glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        // if (m_bIsVSyncEnabled)
        //     setSwapInterval(GLFW_TRUE);
        m_bIsFullScreen = true;
    }
}

void Window_Base::escapeFullScreen()
{
    if (m_bIsFullScreen) {
        GLFWmonitor*       monitor = getCurrentMonitor();
        const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(m_window, NULL, m_posXBeforeFullscreen, m_posYBeforeFullscreen, m_widthBeforeFullscreen, m_heightBeforeFullscreen, mode->refreshRate);
        // if (m_bIsVSyncEnabled)
        //     setSwapInterval(GLFW_TRUE);
        m_bIsFullScreen = false;
    }
}

GLFWmonitor* Window_Base::getCurrentMonitor() const
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

    glfwGetWindowPos(m_window, &wx, &wy);
    glfwGetWindowSize(m_window, &ww, &wh);
    monitors = glfwGetMonitors(&nmonitors);

    for (i = 0; i < nmonitors; i++) {
        mode = glfwGetVideoMode(monitors[i]);
        glfwGetMonitorPos(monitors[i], &mx, &my);
        mw = mode->width;
        mh = mode->height;

        overlap =
            std::max(0, std::min(wx + ww, mx + mw) - std::max(wx, mx)) *
            std::max(0, std::min(wy + wh, my + mh) - std::max(wy, my));

        if (bestoverlap < overlap) {
            bestoverlap = overlap;
            bestmonitor = monitors[i];
        }
    }

    return bestmonitor;
}

} // namespace Cool