#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseScrollEvent {
    // The position of the mouse
    Coords position;
    // Scroll amount in the x direction (Not many mouses can generate it)
    float dx;
    // Scroll amount in the y direction (Most mouses can only generate this one)
    float dy;
};

} // namespace Cool
