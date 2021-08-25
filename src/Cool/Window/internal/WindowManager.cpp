#include "WindowManager.h"

namespace Cool {

Window& WindowManager::find(GLFWwindow* glfw_window)
{
    return *std::find_if(std::begin(_windows), std::end(_windows), [&](const Window& window) {
        return window.glfw() == glfw_window;
    });
}

} // namespace Cool