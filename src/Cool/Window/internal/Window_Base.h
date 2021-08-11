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
    bool check_for_fullscreen_toggles(int key, int scancode, int action, int mods);
    void switch_fullscreen();
    void escape_fullscreen();

    /**
     * @brief Returns the underlying glfw window pointer
     * 
     * @return GLFWwindow* 
     */
    GLFWwindow* glfw() { return _glfw_window; }

    /**
     * @brief Returns the monitor that the window is in
     * (if it is over multiple monitors, returns the one which contains most of the window)
     * 
     * @return GLFWmonitor* 
     */
    GLFWmonitor* current_monitor() const;

private:
    friend class Window_OpenGL;
    friend class Window_Vulkan;
    Window_Base(GLFWwindow* glfw_window);
    Window_Base(Window_Base&&) noexcept;
    Window_Base& operator=(Window_Base&&) noexcept;

private:
    GLFWwindow* _glfw_window;
    bool        _is_fullscreen = false;
    int         _pos_x_before_fullscreen;
    int         _pos_y_before_fullscreen;
    int         _width_before_fullscreen;
    int         _height_before_fullscreen;
};

} // namespace Cool
