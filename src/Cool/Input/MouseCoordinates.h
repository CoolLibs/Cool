#pragma once
#include <GLFW/glfw3.h>
#include <concepts>

namespace Cool {

template<typename T>
concept MouseCoordinates = requires(T coords) {
                               // clang-format off
    { coords } -> std::convertible_to<glm::vec2>;
                               // clang-format on
                           };

class ScreenCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit ScreenCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }
};

class WindowCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit WindowCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }

    ScreenCoordinates as_screen_coordinates(GLFWwindow* window) const
    {
        int xx, yy;
        glfwGetWindowPos(window, &xx, &yy);
        return ScreenCoordinates{*this + glm::vec2{xx, yy}};
    }
};

class ViewCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit ViewCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }
};

} // namespace Cool