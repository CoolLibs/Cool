#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseMoveEvent {
    // The position of the mouse
    Coords position;
    // The difference between the current and previous mouse positions
    Coords delta;
};

} // namespace Cool
