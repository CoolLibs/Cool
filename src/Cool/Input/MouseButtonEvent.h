#pragma once
#include "ModifierKeys.h"
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseButtonEvent {
    // The position of the mouse
    Coords position;
    // Can be GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, GLFW_MOUSE_BUTTON_MIDDLE and some more that you can check at https://www.glfw.org/docs/latest/group__buttons.html
    int button;
    // Can be GLFW_PRESS or GLFW_RELEASE
    int action;
    // Modifier keys that where pressed when the even occured.
    ModifierKeys mods;
};

} // namespace Cool
