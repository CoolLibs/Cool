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
    explicit ScreenCoordinates(glm::vec2 v)
        : glm::vec2{v}
    {
    }
};

class MainWindowCoordinates : public glm::vec2 {
public:
    explicit MainWindowCoordinates(glm::vec2 v)
        : glm::vec2{v}
    {
    }
};

class ImGuiWindowCoordinates : public glm::vec2 {
public:
    explicit ImGuiWindowCoordinates(glm::vec2 v)
        : glm::vec2{v}
    {
    }
};

} // namespace Cool