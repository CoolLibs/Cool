#if defined(COOL_OPENGL)

#include "Window_ImplOpenGL.h"

namespace Cool {

void Window_ImplOpenGL::cap_framerate_if(bool should_cap, GLFWwindow* window)
{
    GLFWwindow* const previous_ctx = glfwGetCurrentContext();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(should_cap ? 1 : 0);
    glfwMakeContextCurrent(previous_ctx);
    _framerate_is_capped = should_cap;
}

} // namespace Cool

#endif