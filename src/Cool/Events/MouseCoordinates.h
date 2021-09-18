#pragma once

namespace Cool {

template<typename T>
concept MouseCoordinates = requires(T coords)
{
    coords.value();
};

class ScreenCoordinates {
public:
    ScreenCoordinates(glm::vec2 value)
        : _value{value}
    {
    }
    glm::vec2 value() const { return _value; }

private:
    glm::vec2 _value;
};

class MainWindowCoordinates {
public:
    MainWindowCoordinates(glm::vec2 value)
        : _value{value}
    {
    }
    glm::vec2 value() const { return _value; }

private:
    glm::vec2 _value;
};

class ImGuiWindowCoordinates {
public:
    ImGuiWindowCoordinates(glm::vec2 value)
        : _value{value}
    {
    }
    glm::vec2 value() const { return _value; }

private:
    glm::vec2 _value;
};

} // namespace Cool