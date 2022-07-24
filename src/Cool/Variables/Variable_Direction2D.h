#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/Direction2D.h>
#include "Variable.h"

namespace Cool {

#include "generated/Variable_Direction2D.inl"

inline auto imgui_widget(Variable<Direction2D>& var) -> bool
{
    return imgui_widget(var.name, var.value);
}

inline auto imgui_widget(VariableMetadata<Direction2D>&) -> bool
{
    return false;
}

} // namespace Cool
