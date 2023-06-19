#pragma once
#include <imgui.h>
#include "MouseCoordinates.h"

namespace Cool {

enum class ButtonAction {
    Pressed,
    Released,
};

template<MouseCoordinates Coords>
struct MouseButtonEvent {
    // The position of the mouse
    Coords           position;
    ImGuiMouseButton button;
    ButtonAction     action;
};

} // namespace Cool
