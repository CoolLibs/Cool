#pragma once

#include <Cool/StrongTypes/Point2D.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Point2D.inl"

inline auto imgui_widget(Variable<Point2D>& var) -> bool
{
    return imgui_widget(
        var.name.c_str(),
        var.value
    );
}

inline auto imgui_widget(VariableMetadata<Point2D>&) -> bool
{
    return false;
}

} // namespace Cool
