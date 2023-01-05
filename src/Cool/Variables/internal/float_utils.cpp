#include "float_utils.h"
#include "imgui.h"

namespace Cool::internal {

auto float_metadata_widget(float& min_value, float& max_value, bool& bounded, float& drag_speed) -> bool
{
    bool b = false;

    if (bounded)
    {
        b |= ImGui::DragFloat("##1", &min_value, 0.01f);
        ImGui::SameLine();
        ImGui::Text("to");
        ImGui::SameLine();
        b |= ImGui::DragFloat("##2", &max_value, 0.01f);
    }
    else
    {
        b |= ImGui::DragFloat("Drag speed", &drag_speed, 0.01f, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
    }

    b |= ImGui::Checkbox("Bounded", &bounded);

    return b;
}

} // namespace Cool::internal