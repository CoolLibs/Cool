#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragStartEvent {
    Coords position;
};

template<MouseCoordinates Coords>
struct MouseDragUpdateEvent {
    Coords position;
    Coords delta;
};

template<MouseCoordinates Coords>
struct MouseDragStopEvent {
    Coords position;
};

} // namespace Cool
