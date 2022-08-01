#pragma once

#include <Cool/StrongTypes/Angle.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Angle.inl"

inline auto imgui_widget(Variable<Angle>& var) -> bool
{
    return Cool::imgui_widget(var.name, var.value);
}

inline auto imgui_widget(VariableMetadata<Angle>&) -> bool
{
    return false;
}

} // namespace Cool
