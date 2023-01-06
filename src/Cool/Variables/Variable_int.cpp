#include "Variable_int.h"
#include "imgui.h"

namespace Cool {

auto imgui_widget(Variable<int>& var) -> bool
{
    if (var.metadata.bounded)
    {
        return ImGui::SliderInt(
            var.name.c_str(),
            &var.value,
            var.metadata.min_value,
            var.metadata.max_value
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return ImGui::DragInt(
            var.name.c_str(),
            &var.value,
            var.metadata.drag_speed
        );
    }
}

auto imgui_widget(VariableMetadata<int>& meta) -> bool
{
    bool b = false;

    if (meta.bounded)
    {
        b |= ImGui::DragInt("##1", &meta.min_value, 0.01f);
        ImGui::SameLine();
        ImGui::Text("to");
        ImGui::SameLine();
        b |= ImGui::DragInt("##2", &meta.max_value, 0.01f);
    }
    else
    {
        b |= ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.01f, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
    }

    b |= ImGui::Checkbox("Bounded", &meta.bounded);

    return b;
}

} // namespace Cool