#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragUpdateEvent {
    Coords position;
};

} // namespace Cool
