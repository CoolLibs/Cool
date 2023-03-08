#include "WindowManager.h"

namespace Cool {

auto WindowManager::find(GLFWwindow* glfw_window) -> Window&
{
    return *std::find_if(std::begin(_windows), std::end(_windows), [&](Window const& window) {
        return window.glfw() == glfw_window;
    });
}

} // namespace Cool