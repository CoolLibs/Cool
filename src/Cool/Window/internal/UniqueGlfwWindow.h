#pragma once
#include <GLFW/glfw3.h>

namespace Cool {

class UniqueGlfwWindow {
public:
    explicit UniqueGlfwWindow(GLFWwindow* window);
    ~UniqueGlfwWindow();
    UniqueGlfwWindow(const UniqueGlfwWindow&)                    = delete;
    auto operator=(const UniqueGlfwWindow&) -> UniqueGlfwWindow& = delete;
    UniqueGlfwWindow(UniqueGlfwWindow&& rhs) noexcept;
    auto operator=(UniqueGlfwWindow&& rhs) noexcept -> UniqueGlfwWindow&;

    auto operator*() const -> GLFWwindow* { return _window; }

private:
    GLFWwindow* _window{};
};

} // namespace Cool
