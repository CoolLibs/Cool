#pragma once
#include "EventDispatcher.h"
#include "MouseDragStartEvent.h"
#include "MouseDragStopEvent.h"
#include "MouseDragUpdateEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
class MouseDragManager {
public:
    auto start() -> EventDispatcher<MouseDragStartEvent<Coords>>& { return _drag_start_dispatcher; }
    auto update() -> EventDispatcher<MouseDragUpdateEvent<Coords>>& { return _drag_update_dispatcher; }
    auto stop() -> EventDispatcher<MouseDragStopEvent<Coords>>& { return _drag_stop_dispatcher; }

    void receive_mouse_button_event(const MouseButtonEvent<ImGuiWindowCoordinates>& event, bool is_inside_view)
    {
        if (event.action == GLFW_PRESS && is_inside_view && !_is_dragging) {
            _is_dragging = true;
            start().receive({event.position});
        }
        if (event.action == GLFW_RELEASE && _is_dragging) {
            _is_dragging = false;
            stop().receive({event.position});
        }
    }

private:
    bool                                          _is_dragging = false;
    EventDispatcher<MouseDragStartEvent<Coords>>  _drag_start_dispatcher;
    EventDispatcher<MouseDragUpdateEvent<Coords>> _drag_update_dispatcher;
    EventDispatcher<MouseDragStopEvent<Coords>>   _drag_stop_dispatcher;
};

} // namespace Cool
