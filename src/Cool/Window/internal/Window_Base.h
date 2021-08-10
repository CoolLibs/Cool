#pragma once

#include <GLFW/glfw3.h>

namespace Cool {

class Window_Base {
public:
    /// <summary>
    /// Please use WindowFactory::create() to create a Window
    /// </summary>
    Window_Base(GLFWwindow* m_window);
    Window_Base(Window_Base&&) noexcept;
    Window_Base& operator=(Window_Base&&) noexcept;

    Window_Base(const Window_Base&) = delete;            // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
    Window_Base& operator=(const Window_Base&) = delete; // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
    void         destroy();

    /// <summary>
    /// You have to forward the parameters of the IApp::onKeyboardEvent() function (a.k.a. glfw key callback) and it will turn fullscreen on / off appropriately.
    /// </summary>
    /// <returns>true iff something has changed</returns>
    bool checkForFullscreenToggles(int key, int scancode, int action, int mods);
    void switchFullScreen();
    void escapeFullScreen();
    /// <summary>
    /// Returns the underlying glfw window pointer
    /// </summary>
    GLFWwindow* glfw() { return m_window; }
    /// <summary>
    /// Returns the monitor that the window is mostly displayed in
    /// </summary>
    GLFWmonitor* getCurrentMonitor() const;

private:
    GLFWwindow* m_window;
    bool        m_bIsFullScreen = false;
    int         m_posXBeforeFullscreen;
    int         m_posYBeforeFullscreen;
    int         m_widthBeforeFullscreen;
    int         m_heightBeforeFullscreen;
};

} // namespace Cool
