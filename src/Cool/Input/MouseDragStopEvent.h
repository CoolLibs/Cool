#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragStopEvent {
    Coords position;
    int    mods;
};

} // namespace Cool
