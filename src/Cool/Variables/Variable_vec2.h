#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_vec2.inl"

inline auto imgui_widget(Variable<glm::vec2>& var) -> bool
{
    return ImGui::DragFloat2(
        var.name.c_str(),
        glm::value_ptr(var.value),
        var.metadata.drag_speed
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec2>& meta) -> bool
{
    return ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.01f, 0.001f, FLT_MAX / INT_MAX);
}

} // namespace Cool
