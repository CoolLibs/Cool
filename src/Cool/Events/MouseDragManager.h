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
        if (event.action == GLFW_PRESS && is_inside_view && !_dragged_button.has_value()) {
            _dragged_button.emplace(event.button);
            start().receive({event.position});
        }
        if (event.action == GLFW_RELEASE && _dragged_button.has_value() && *_dragged_button == event.button) {
            _dragged_button.reset();
            stop().receive({event.position});
        }
    }

private:
    std::optional<int>                            _dragged_button{};
    EventDispatcher<MouseDragStartEvent<Coords>>  _drag_start_dispatcher;
    EventDispatcher<MouseDragUpdateEvent<Coords>> _drag_update_dispatcher;
    EventDispatcher<MouseDragStopEvent<Coords>>   _drag_stop_dispatcher;
};

} // namespace Cool
