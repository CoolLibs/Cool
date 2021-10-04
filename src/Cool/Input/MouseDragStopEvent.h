#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragStopEvent {
    Coords       position;
    ModifierKeys mods;
};

} // namespace Cool
