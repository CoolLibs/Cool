#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_vec2.inl"

inline auto imgui_widget(Variable<glm::vec2>& var) -> bool
{
    if (var.metadata.bounded)
    {
        return ImGui::SliderFloat2(
            var.name.c_str(),
            glm::value_ptr(var.value),
            var.metadata.min_value,
            var.metadata.max_value
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return ImGui::DragFloat2(
            var.name.c_str(),
            glm::value_ptr(var.value),
            var.metadata.drag_speed
        );
    }
}

inline auto imgui_widget(VariableMetadata<glm::vec2>& meta) -> bool
{
    return internal::float_metadata_widget(meta.min_value, meta.max_value, meta.bounded, meta.drag_speed);
}

} // namespace Cool
