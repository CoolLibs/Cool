#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseScrollEvent {
    Coords position;
    float  dx;
    float  dy;
};

} // namespace Cool
