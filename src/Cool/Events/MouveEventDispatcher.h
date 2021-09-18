#pragma once

#include "EventDispatcher.h"
#include "MouseMoveEvent.h"
#include "MouseScrollEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
class MouveEventDispatcher {
public:
    EventDispatcher<MouseMoveEvent<Coords>>& move_event() { return _mouse_move_dispatcher; }
    EventDispatcher<MouseScrollEvent>&       scroll_event() { return _mouse_scroll_dispatcher; }

private:
    EventDispatcher<MouseMoveEvent<Coords>> _mouse_move_dispatcher;
    EventDispatcher<MouseScrollEvent>       _mouse_scroll_dispatcher;
};

} // namespace Cool
