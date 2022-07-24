#pragma once

#include <Cool/StrongTypes/Hue.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Hue.inl"

inline auto imgui_widget(Variable<Hue>& var) -> bool
{
    return imgui_widget(var.name, var.value);
}

inline auto imgui_widget(VariableMetadata<Hue>&) -> bool
{
    return false;
}

} // namespace Cool
