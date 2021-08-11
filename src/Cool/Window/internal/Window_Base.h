#pragma once

#include <GLFW/glfw3.h>

namespace Cool {

class Window_Base {
public:
    Window_Base(const Window_Base&) = delete;            // Non-copyable because there should only be one owner of the window.
    Window_Base& operator=(const Window_Base&) = delete; // Non-copyable because there should only be one owner of the window.
    Window_Base::~Window_Base();

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
    friend class Window_OpenGL;
    friend class Window_Vulkan;
    Window_Base(GLFWwindow* m_window);
    Window_Base(Window_Base&&) noexcept;
    Window_Base& operator=(Window_Base&&) noexcept;

private:
    GLFWwindow* m_window;
    bool        m_bIsFullScreen = false;
    int         m_posXBeforeFullscreen;
    int         m_posYBeforeFullscreen;
    int         m_widthBeforeFullscreen;
    int         m_heightBeforeFullscreen;
};

} // namespace Cool
