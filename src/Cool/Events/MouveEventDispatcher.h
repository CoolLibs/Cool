#pragma once

#include "EventDispatcher.h"
#include "MouseMoveEvent.h"

namespace Cool {

class MouveEventDispatcher {
public:
    EventDispatcher<MouseMoveEvent>& move_event() { return _mouse_move_dispatcher; }

private:
    EventDispatcher<MouseMoveEvent> _mouse_move_dispatcher;
};

} // namespace Cool
