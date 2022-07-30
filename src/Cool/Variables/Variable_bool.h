#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_bool.inl"

inline auto imgui_widget(Variable<bool>& var) -> bool
{
    return ImGui::Checkbox(var.name.c_str(), &var.value);
}

inline auto imgui_widget(VariableMetadata<bool>&) -> bool
{
    return false;
}

} // namespace Cool
