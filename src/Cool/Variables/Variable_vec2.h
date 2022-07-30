#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_vec2.inl"

inline auto imgui_widget(Variable<glm::vec2>& var) -> bool
{
    return ImGui::DragFloat2(
        var.name.c_str(),
        glm::value_ptr(var.value)
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec2>&) -> bool
{
    return false;
}

} // namespace Cool
