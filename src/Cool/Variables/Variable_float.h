#pragma once

#include "Cool/Variables/internal/float_utils.h"
#include "Variable.h"

namespace Cool {

#include "generated/Variable_float.inl"

inline auto imgui_widget(Variable<float>& var) -> bool
{
    if (var.metadata.bounded)
    {
        return ImGui::SliderFloat(
            var.name.c_str(),
            &var.value,
            var.metadata.min_value,
            var.metadata.max_value
        );
    }
    else
    {
        return ImGui::DragFloat(
            var.name.c_str(),
            &var.value,
            var.metadata.drag_speed
        );
    }
}

inline auto imgui_widget(VariableMetadata<float>& meta) -> bool
{
    return internal::float_metadata_widget(meta.min_value, meta.max_value, meta.bounded, meta.drag_speed);
}

} // namespace Cool
