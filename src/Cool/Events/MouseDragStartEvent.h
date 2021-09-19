#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragStartEvent {
    Coords position;
};

} // namespace Cool
