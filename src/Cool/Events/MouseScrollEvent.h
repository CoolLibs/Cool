#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseScrollEvent {
    float  dx;
    float  dy;
    Coords position;
};

} // namespace Cool
