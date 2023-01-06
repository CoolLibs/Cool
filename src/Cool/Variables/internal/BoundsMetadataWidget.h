#pragma once
#include <Cool/ImGui/ImGuiExtras.h>
#include "BoundsMetadata.h"
#include "imgui.h"

namespace Cool::internal {

inline auto imgui_drag(const char* label, int* val, float drag_speed) -> bool
{
    return ImGui::DragInt(label, val, drag_speed);
}
inline auto imgui_drag(const char* label, float* val, float drag_speed) -> bool
{
    return ImGui::DragFloat(label, val, drag_speed);
}

inline auto imgui_drag(const char* label, int* val, float drag_speed, const char* text_in_input) -> bool
{
    return ImGui::DragInt(label, val, drag_speed, 0, 0, text_in_input);
}
inline auto imgui_drag(const char* label, float* val, float drag_speed, const char* text_in_input) -> bool
{
    return ImGui::DragFloat(label, val, drag_speed, 0.f, 0.f, text_in_input);
}

inline auto imgui_drag(const char* label, int* val, float drag_speed, int min, int max) -> bool
{
    return ImGui::DragInt(label, val, drag_speed, min, max);
}
inline auto imgui_drag(const char* label, float* val, float drag_speed, float min, float max) -> bool
{
    return ImGui::DragFloat(label, val, drag_speed, min, max);
}
inline auto imgui_drag(const char* label, glm::vec2* val, float drag_speed, float min, float max) -> bool
{
    return ImGui::DragFloat2(label, glm::value_ptr(*val), drag_speed, min, max);
}
inline auto imgui_drag(const char* label, glm::vec3* val, float drag_speed, float min, float max) -> bool
{
    return ImGui::DragFloat3(label, glm::value_ptr(*val), drag_speed, min, max);
}
inline auto imgui_drag(const char* label, glm::vec4* val, float drag_speed, float min, float max) -> bool
{
    return ImGui::DragFloat4(label, glm::value_ptr(*val), drag_speed, min, max);
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

template<typename T>
static constexpr auto biggest_number() -> T;

template<>
constexpr auto biggest_number<float>() -> float
{
    return FLT_MAX / static_cast<float>(INT_MAX);
}

template<>
constexpr auto biggest_number<int>() -> int
{
    return INT_MAX - 1; // - 1 to make sure it won't overflow even when we flip the sign.
}

/// E.g. ExactType is glm::vec3 and BaseType is float.
template<typename ExactType, typename BaseType>
auto imgui_widget(const char* label, ExactType* value, BoundsMetadata<BaseType> const& bounds) -> bool
{
    if (bounds.use_slider)
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
            bounds.drag_speed,
            bounds.has_min_bound ? bounds.min : -biggest_number<BaseType>(),
            bounds.has_max_bound ? bounds.max : +biggest_number<BaseType>()
        );
    }
}

template<typename T>
auto imgui_widget(BoundsMetadata<T>& meta) -> bool
{
    bool b = false;

    if (meta.use_slider)
    {
        b |= imgui_drag("##1", &meta.min, 0.01f);
        ImGui::SameLine();
        ImGui::TextUnformatted("to");
        ImGui::SameLine();
        b |= imgui_drag("##2", &meta.max, 0.01f);
    }
    else
    {
        b |= ImGui::Checkbox("##1", &meta.has_min_bound);
        ImGui::SameLine();

        if (meta.has_min_bound)
        {
            b |= imgui_drag("##2", &meta.min, 0.01f);
        }
        else
        {
            ImGuiExtras::maybe_disabled(true, "Toggle the checkbox on the left to set a min value.", [&]() {
                b |= imgui_drag("##3", &meta.min, 0.01f, "-infinity");
            });
        }

        ImGui::SameLine();
        ImGui::TextDisabled("to");
        ImGui::SameLine();

        b |= ImGui::Checkbox("##4", &meta.has_max_bound);
        ImGui::SameLine();

        if (meta.has_max_bound)
        {
            b |= imgui_drag("##5", &meta.max, 0.01f);
        }
        else
        {
            ImGuiExtras::maybe_disabled(true, "Toggle the checkbox on the left to set a max value.", [&]() {
                b |= imgui_drag("##6", &meta.max, 0.01f, "+infinity");
            });
        }

        b |= ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.0001f, 0.000001f, biggest_number<float>(), "%.6f");
    }

    b |= ImGui::Checkbox("Use a slider", &meta.use_slider);

    return b;
}

} // namespace Cool::internal