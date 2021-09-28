#pragma once
#include "MouseCoordinates.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragUpdateEvent {
    Coords    position;
    glm::vec2 delta;
};

} // namespace Cool
