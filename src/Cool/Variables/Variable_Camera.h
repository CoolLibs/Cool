#pragma once

#include <Cool/Camera/Camera.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Camera.inl"

inline auto imgui_widget(Variable<Cool::Camera>&) -> bool
{
    return false;
}

inline auto imgui_widget(VariableMetadata<Cool::Camera>&) -> bool
{
    return false;
}

} // namespace Cool
