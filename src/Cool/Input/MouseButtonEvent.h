#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseButtonEvent {
    Coords position;
    int    button;
    int    action;
    int    mods;
};

} // namespace Cool
