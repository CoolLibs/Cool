#pragma once

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
    static constexpr auto drag_speed = 0.01f;

    bool b = false;

    if (meta.bounded)
    {
        b |= ImGui::DragFloat("##1", &meta.min_value, drag_speed);
        ImGui::SameLine();
        ImGui::Text("to");
        ImGui::SameLine();
        b |= ImGui::DragFloat("##2", &meta.max_value, drag_speed);
    }
    else
    {
        b |= ImGui::DragFloat("Drag speed", &meta.drag_speed, drag_speed, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
    }

    b |= ImGui::Checkbox("Bounded", &meta.bounded);

    return b;
}

} // namespace Cool
