#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_vec4.inl"

inline auto imgui_widget(Variable<glm::vec4>& var) -> bool
{
    return ImGui::DragFloat4(
        var.name.c_str(),
        glm::value_ptr(var.value)
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec4>&) -> bool
{
    return false;
}

} // namespace Cool
