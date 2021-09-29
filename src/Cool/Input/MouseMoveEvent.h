#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseMoveEvent {
    // The position of the mouse
    Coords position;
};

} // namespace Cool
