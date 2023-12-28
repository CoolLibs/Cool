#pragma once
#include <Cool/EventDispatcher/EventDispatcher.h>
#include <imgui.h>
#include <optional>
#include "Cool/Input/MouseButtonEvent.h"
#include "MouseDragEvents.h"
#include "MouseMoveEvent.h"

namespace Cool {

template<MouseCoordinates Coords>
struct MouseDragCallbacks {
    /// Returns true iff you decided to start dragging. This prevents other listeners from receiving the drag event.
    std::function<bool(MouseDragStartEvent<Coords>)>  on_start;
    std::function<void(MouseDragUpdateEvent<Coords>)> on_update;
    std::function<void(MouseDragStopEvent<Coords>)>   on_stop;
};

template<MouseCoordinates Coords>
struct DragState {
    size_t           listener_index{};
    ImGuiMouseButton button{};
};

template<MouseCoordinates Coords>
class MouseDragManager {
public:
    /// Note that events will be dispatched to the listeners in the order they have been subscribed.
    /// When a listener reacts to a drag start event, this will prevent subsequent listeners from seeing the event.
    /// This ensures that there will only be one thing dragged at once.
    void subscribe(MouseDragCallbacks<Coords> callbacks)
    {
        _listeners_stack.emplace_back(std::move(callbacks));
    }

    void dispatch_mouse_button_event(MouseButtonEvent<Coords> const& event, bool is_inside_view)
    {
        if (!_drag_state.has_value())
        {
            dispatch_drag_start(event, is_inside_view);
        }
        else
        {
            dispatch_drag_stop(event);
        }
    }

    void dispatch_mouse_move_event(MouseMoveEvent<Coords> const& event)
    {
        if (!_drag_state.has_value())
            return;

        dragged_listener().on_update({
            event.position,
            event.delta,
        });
    }

private:
    auto dragged_listener() -> MouseDragCallbacks<Coords> const&
    {
        assert(_drag_state.has_value());
        return _listeners_stack.at(_drag_state->listener_index);
    }

    void dispatch_drag_start(MouseButtonEvent<Coords> const& event, bool is_inside_view)
    {
        if (event.action != ButtonAction::Pressed)
            return;
        if (!is_inside_view)
            return;

        for (size_t i = 0; i < _listeners_stack.size(); ++i)
        {
            if (_listeners_stack[i].on_start({event.position}))
            {
                _drag_state.emplace(DragState<Coords>{
                    .listener_index = i,
                    .button         = event.button,
                });
                return;
            }
        }
    }

    void dispatch_drag_stop(MouseButtonEvent<Coords> const& event)
    {
        if (event.action != ButtonAction::Released)
            return;
        if (_drag_state->button != event.button)
            return;

        dragged_listener().on_stop({event.position});
        _drag_state.reset();
    }

private:
    std::optional<DragState<Coords>>        _drag_state{};
    std::vector<MouseDragCallbacks<Coords>> _listeners_stack{};
};

} // namespace Cool
