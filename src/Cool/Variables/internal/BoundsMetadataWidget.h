#pragma once
#include "BoundsMetadata.h"

namespace Cool::internal {

template<typename T>
auto imgui_widget(BoundsMetadata<T>& meta) -> bool
{
    bool b = false;

    if (meta.is_bounded)
    {
        b |= ImGui::DragInt("##1", &meta.min, 0.01f);
        ImGui::SameLine();
        ImGui::Text("to");
        ImGui::SameLine();
        b |= ImGui::DragInt("##2", &meta.max, 0.01f);
    }
    else
    {
        b |= ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.01f, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
    }

    b |= ImGui::Checkbox("Bounded", &meta.is_bounded);

    return b;
}

} // namespace Cool::internal