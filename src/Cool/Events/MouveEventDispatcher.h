#pragma once
#include "EventDispatcher.h"
#include "MouseButtonEvent.h"
#include "MouseMoveEvent.h"
#include "MouseScrollEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
class MouveEventDispatcher {
public:
    EventDispatcher<MouseMoveEvent<Coords>>&   move_event() { return _mouse_move_dispatcher; }
    EventDispatcher<MouseScrollEvent<Coords>>& scroll_event() { return _mouse_scroll_dispatcher; }
    EventDispatcher<MouseButtonEvent<Coords>>& button_event() { return _mouse_button_dispatcher; }

private:
    EventDispatcher<MouseMoveEvent<Coords>>   _mouse_move_dispatcher;
    EventDispatcher<MouseScrollEvent<Coords>> _mouse_scroll_dispatcher;
    EventDispatcher<MouseButtonEvent<Coords>> _mouse_button_dispatcher;
};

} // namespace Cool
