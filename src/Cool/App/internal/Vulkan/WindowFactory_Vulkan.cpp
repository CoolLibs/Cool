#include "WindowFactory_Vulkan.h"
#include <GLFW/glfw3.h>

namespace Cool {

void WindowFactory_Vulkan::initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

WindowFactory_Vulkan::WindowFactory_Vulkan()
    : _vku_framework{""}
{
    if (!_vku_framework.ok()) {
        std::cout << "Framework creation failed" << std::endl;
        exit(1);
    }
}

WindowFactory_Vulkan::~WindowFactory_Vulkan()
{
    _vku_framework.device().waitIdle();
    for (GLFWwindow* window : _windows) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

Cool::Window WindowFactory_Vulkan::make_window(const char* title, int width, int height)
{
    GLFWwindow* glfw_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    _windows.push_back(glfw_window);
    return Cool::Window{glfw_window, _vku_framework};
}

} // namespace Cool