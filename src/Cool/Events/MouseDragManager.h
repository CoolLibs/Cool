#pragma once
#include "EventDispatcher.h"
#include "MouseDragStartEvent.h"
#include "MouseDragStopEvent.h"
#include "MouseDragUpdateEvent.h"
#include "MouseMoveEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
class MouseDragManager {
public:
    auto start() -> EventDispatcher<MouseDragStartEvent<Coords>>& { return _drag_start_dispatcher; }
    auto update() -> EventDispatcher<MouseDragUpdateEvent<Coords>>& { return _drag_update_dispatcher; }
    auto stop() -> EventDispatcher<MouseDragStopEvent<Coords>>& { return _drag_stop_dispatcher; }

    void dispatch_mouse_button_event(const MouseButtonEvent<Coords>& event, bool is_inside_view)
    {
        if (event.action == GLFW_PRESS && is_inside_view && !_dragged_button.has_value()) {
            _dragged_button.emplace(event.button);
            _last_mouse_position = event.position;
            start().dispatch({event.position, event.mods});
        }
        if (event.action == GLFW_RELEASE && _dragged_button.has_value() && *_dragged_button == event.button) {
            _dragged_button.reset();
            stop().dispatch({event.position, event.mods});
        }
    }

    void dispatch_mouse_move_event(const MouseMoveEvent<Coords>& event)
    {
        if (_dragged_button.has_value()) {
            update().dispatch({event.position,
                               event.position - _last_mouse_position});
            _last_mouse_position = event.position;
        }
    }

private:
    std::optional<int>                            _dragged_button{};
    Coords                                        _last_mouse_position;
    EventDispatcher<MouseDragStartEvent<Coords>>  _drag_start_dispatcher;
    EventDispatcher<MouseDragUpdateEvent<Coords>> _drag_update_dispatcher;
    EventDispatcher<MouseDragStopEvent<Coords>>   _drag_stop_dispatcher;
};

} // namespace Cool
