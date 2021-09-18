#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseMoveEvent {
    Coords position;
};

} // namespace Cool
