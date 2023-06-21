#pragma once
#include <reg/src/AnyId.hpp>
#include "Cool/Input/MouseCoordinates.h"

namespace Cool {

struct Gizmo_Point2D {
    ViewCoordinates position;
    reg::AnyId      _id;

    [[nodiscard]] auto id() const -> reg::AnyId { return _id; }
};

} // namespace Cool