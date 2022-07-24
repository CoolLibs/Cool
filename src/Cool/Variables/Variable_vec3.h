#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_vec3.inl"

inline auto imgui_widget(Variable<glm::vec3>& var) -> bool
{
    return ImGui::DragFloat3(
        var.name.c_str(),
        glm::value_ptr(var.value)
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec3>&) -> bool
{
    return false;
}

} // namespace Cool
