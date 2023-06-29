#pragma once
#include <reg/src/AnyId.hpp>
#include "Cool/Input/MouseCoordinates.h"

namespace Cool {

struct Gizmo_Point2D {
    std::function<ViewCoordinates()>     get_position;
    std::function<void(ViewCoordinates)> set_position;
    std::function<void()>                on_drag_stop;
    reg::AnyId                           id;
};

} // namespace Cool