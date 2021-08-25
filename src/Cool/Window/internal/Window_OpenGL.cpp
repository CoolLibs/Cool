#if defined(__COOL_APP_OPENGL)

#include "Window_OpenGL.h"

namespace Cool {

void Window_OpenGL::cap_framerate(bool should_cap)
{
    GLFWwindow* current_glfw = glfwGetCurrentContext();
    glfwMakeContextCurrent(glfw());
    glfwSwapInterval(should_cap ? 1 : 0);
    glfwMakeContextCurrent(current_glfw);
    _framerate_is_capped = should_cap;
}

} // namespace Cool

#endif