#include "UniqueGlfwWindow.h"

namespace Cool {

UniqueGlfwWindow::UniqueGlfwWindow(GLFWwindow* window)
    : _window{window}
{
}

UniqueGlfwWindow::~UniqueGlfwWindow()
{
    if (_window) // Could have been moved
        glfwDestroyWindow(_window);
}

UniqueGlfwWindow::UniqueGlfwWindow(UniqueGlfwWindow&& rhs) noexcept
    : _window{rhs._window}
{
    rhs._window = nullptr;
}

auto UniqueGlfwWindow::operator=(UniqueGlfwWindow&& rhs) noexcept -> UniqueGlfwWindow&
{
    if (this != &rhs)
    {
        _window     = rhs._window;
        rhs._window = nullptr;
    }
    return *this;
}

} // namespace Cool