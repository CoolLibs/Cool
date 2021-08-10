#if defined(__COOL_APP_OPENGL)

#include "Window_OpenGL.h"

namespace Cool {

void Window_OpenGL::enableVSync()
{
    setSwapInterval(GLFW_TRUE);
    m_bIsVSyncEnabled = true;
}

void Window_OpenGL::disableVSync()
{
    setSwapInterval(GLFW_FALSE);
    m_bIsVSyncEnabled = false;
}

void Window_OpenGL::setSwapInterval(int value)
{
    //GLFWwindow* currentWindow = glfwGetCurrentContext();
    //glfwMakeContextCurrent(m_window);
    //glfwSwapInterval(value);
    //glfwMakeContextCurrent(currentWindow);
}

} // namespace Cool

#endif