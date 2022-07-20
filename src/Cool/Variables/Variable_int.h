#pragma once

#include "Variable.h"

namespace Cool {

#include "generated/Variable_int.inl"

inline auto imgui_widget(Variable<int>& var) -> bool
{
    return ImGui::SliderInt(
        var.name.c_str(),
        &var.value,
        var.metadata.min_value,
        var.metadata.max_value
    );
}

inline auto imgui_widget(VariableMetadata<int>& meta) -> bool
{
    bool b = false;
    b |= ImGui::DragInt("##1", &meta.min_value);
    ImGui::SameLine();
    ImGui::Text("to");
    ImGui::SameLine();
    b |= ImGui::DragInt("##2", &meta.max_value);
    return b;
}

} // namespace Cool
