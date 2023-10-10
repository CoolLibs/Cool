#pragma once
#include <GLFW/glfw3.h>
#include "internal/UniqueGlfwWindow.h"
#include "internal/Window_Impl.h"

namespace Cool {

class Window {
public:
    explicit Window(GLFWwindow* glfw_window);

    /// You have to forward the parameters of the glfw key callback and it will turn fullscreen on / off appropriately.
    /// Returns true iff something has changed
    auto check_for_fullscreen_toggles(int key, int scancode, int action, int mods) -> bool;
    void toggle_fullscreen();
    void set_fullscreen(bool is_fullscreen);
    void escape_fullscreen();
    void turn_on_fullscreen();

    [[nodiscard]] auto is_visible() const -> bool { return _is_visible; }
    void               set_visibility(bool is_visible);

    /// Adds an imgui widget that allows users to choose if the framerate should be capped or not
    /// Returns true iff the widget was triggered
    auto               imgui_cap_framerate() -> bool;
    void               cap_framerate_if(bool should_cap);
    [[nodiscard]] auto framerate_is_capped() const -> bool;

    /// Returns the underlying glfw window pointer
    [[nodiscard]] auto glfw() const -> GLFWwindow* { return *_glfw_window; }

    /// Returns the monitor that the window is in
    /// if it is over multiple monitors, returns the one which contains most of the window)
    [[nodiscard]] auto current_monitor() const -> GLFWmonitor*;

private:
    UniqueGlfwWindow _glfw_window;
    bool             _is_fullscreen{false};
    bool             _is_visible{true};
    int              _pos_x_before_fullscreen{};
    int              _pos_y_before_fullscreen{};
    int              _width_before_fullscreen{};
    int              _height_before_fullscreen{};

    Window_Impl _impl{};
};

} // namespace Cool
