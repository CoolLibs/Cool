#pragma once

#include "EventDispatcher.h"
#include "MouseMoveEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
class MouveEventDispatcher {
public:
    EventDispatcher<MouseMoveEvent<Coords>>& move_event() { return _mouse_move_dispatcher; }

private:
    EventDispatcher<MouseMoveEvent<Coords>> _mouse_move_dispatcher;
};

} // namespace Cool
