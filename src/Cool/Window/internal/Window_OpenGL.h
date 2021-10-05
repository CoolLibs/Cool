#pragma once
#if defined(COOL_OPENGL)

#include "Window_Base.h"

namespace Cool {

class Window_OpenGL : public Window_Base {
public:
    // To construct a window, use WindowFactory_OpenGL::make_window()
    explicit Window_OpenGL(GLFWwindow* window);
    Window_OpenGL(Window_OpenGL&&) noexcept = default;
    Window_OpenGL& operator=(Window_OpenGL&&) noexcept = default;

    inline void make_current() { glfwMakeContextCurrent(glfw()); }

    void cap_framerate_if(bool should_cap) override;
    bool framerate_is_capped() const override { return _framerate_is_capped; }

private:
    bool _framerate_is_capped = true;
};

} // namespace Cool

#endif