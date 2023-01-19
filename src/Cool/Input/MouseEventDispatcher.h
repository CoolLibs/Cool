#pragma once
#include <Cool/EventDispatcher/EventDispatcher.h>
#include "MouseButtonEvent.h"
#include "MouseDragManager.h"
#include "MouseMoveEvent.h"
#include "MouseScrollEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
class MouseEventDispatcher {
public:
    EventDispatcher<MouseMoveEvent<Coords>>&   move_event() { return _mouse_move_dispatcher; }
    EventDispatcher<MouseScrollEvent<Coords>>& scroll_event() { return _mouse_scroll_dispatcher; }
    EventDispatcher<MouseButtonEvent<Coords>>& button_event() { return _mouse_button_dispatcher; }
    MouseDragManager<Coords>&                  drag() { return _mouse_drag_manager; }

private:
    EventDispatcher<MouseMoveEvent<Coords>>   _mouse_move_dispatcher;
    EventDispatcher<MouseScrollEvent<Coords>> _mouse_scroll_dispatcher;
    EventDispatcher<MouseButtonEvent<Coords>> _mouse_button_dispatcher;
    MouseDragManager<Coords>                  _mouse_drag_manager;
};

} // namespace Cool
