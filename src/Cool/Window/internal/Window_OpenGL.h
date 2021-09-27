#pragma once
#if defined(__COOL_APP_OPENGL)

#include "Window_Base.h"

namespace Cool {

class Window_OpenGL : public Window_Base {
public:
    Window_OpenGL(Window_OpenGL&&) noexcept = default;
    Window_OpenGL& operator=(Window_OpenGL&&) noexcept = default;

    inline void make_current() { glfwMakeContextCurrent(glfw()); }

    void cap_framerate(bool should_cap) override;
    bool framerate_is_capped() const override { return _framerate_is_capped; }

private:
    // To construct a window, use WindowFactory_OpenGL::make_window()
    friend class WindowFactory_OpenGL;
    Window_OpenGL(GLFWwindow* window);

private:
    bool _framerate_is_capped = true;
};

} // namespace Cool

#endif