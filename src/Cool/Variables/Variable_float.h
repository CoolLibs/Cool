#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_float.inl"

inline auto imgui_widget(Variable<float>& var) -> bool
{
    return ImGui::SliderFloat(
        var.name.c_str(),
        &var.value,
        var.metadata.min_value,
        var.metadata.max_value
    );
}

inline auto imgui_widget(VariableMetadata<float>& meta) -> bool
{
    static constexpr auto drag_speed = 0.01f;

    bool b = false;
    b |= ImGui::DragFloat("##1", &meta.min_value, drag_speed);
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    b |= ImGui::DragFloat("##2", &meta.max_value, drag_speed);

    return b;
}

} // namespace Cool
