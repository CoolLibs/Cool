#pragma once
#include <concepts>

namespace Cool {

template<typename T>
concept MouseCoordinates = requires(T coords)
{
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

class MainWindowCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit MainWindowCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }
};

class ImGuiWindowCoordinates : public glm::vec2 {
public:
    template<typename... Args>
    explicit ImGuiWindowCoordinates(Args... args)
        : glm::vec2{std::forward<Args>(args)...}
    {
    }
};

} // namespace Cool