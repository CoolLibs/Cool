#pragma once
#include "BoundsMetadata.h"

namespace Cool::internal {

inline auto imgui_drag(const char* label, int* val, float drag_speed) -> bool
{
    return ImGui::DragInt(label, val, drag_speed);
}
inline auto imgui_drag(const char* label, float* val, float drag_speed) -> bool
{
    return ImGui::DragFloat(label, val, drag_speed);
}
inline auto imgui_drag(const char* label, glm::vec2* val, float drag_speed) -> bool
{
    return ImGui::DragFloat2(label, glm::value_ptr(*val), drag_speed);
}
inline auto imgui_drag(const char* label, glm::vec3* val, float drag_speed) -> bool
{
    return ImGui::DragFloat3(label, glm::value_ptr(*val), drag_speed);
}
inline auto imgui_drag(const char* label, glm::vec4* val, float drag_speed) -> bool
{
    return ImGui::DragFloat4(label, glm::value_ptr(*val), drag_speed);
}

inline auto imgui_slider(const char* label, int* val, int min, int max) -> bool
{
    return ImGui::SliderInt(label, val, min, max);
}
inline auto imgui_slider(const char* label, float* val, float min, float max) -> bool
{
    return ImGui::SliderFloat(label, val, min, max);
}
inline auto imgui_slider(const char* label, glm::vec2* val, float min, float max) -> bool
{
    return ImGui::SliderFloat2(label, glm::value_ptr(*val), min, max);
}
inline auto imgui_slider(const char* label, glm::vec3* val, float min, float max) -> bool
{
    return ImGui::SliderFloat3(label, glm::value_ptr(*val), min, max);
}
inline auto imgui_slider(const char* label, glm::vec4* val, float min, float max) -> bool
{
    return ImGui::SliderFloat4(label, glm::value_ptr(*val), min, max);
}

/// E.g. ExactType is glm::vec3 and BaseType is float.
template<typename ExactType, typename BaseType>
auto imgui_widget(const char* label, ExactType* value, BoundsMetadata<BaseType> const& bounds) -> bool
{
    if (bounds.is_bounded)
    {
        return imgui_slider(
            label,
            value,
            bounds.min,
            bounds.max
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return imgui_drag(
            label,
            value,
            bounds.drag_speed
        );
    }
}

template<typename T>
auto imgui_widget(BoundsMetadata<T>& meta) -> bool
{
    bool b = false;

    if (meta.is_bounded)
    {
        b |= imgui_drag("##1", &meta.min, 0.01f);
        ImGui::SameLine();
        ImGui::Text("to");
        ImGui::SameLine();
        b |= imgui_drag("##2", &meta.max, 0.01f);
    }
    else
    {
        b |= ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.01f, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
    }

    b |= ImGui::Checkbox("Bounded", &meta.is_bounded);

    return b;
}

} // namespace Cool::internal