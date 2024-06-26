#pragma once
#include <Cool/ImGui/ImGuiExtras.h>
#include <glm/gtc/matrix_access.hpp>
#include "BoundsMetadata.h"
#include "glm/matrix.hpp"
#include "imgui.h"

namespace Cool::internal {

inline auto imgui_drag(const char* label, int* val, float drag_speed, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragInt(label, val, drag_speed, 0, 0, format.value_or("%d"), flags);
}
inline auto imgui_drag(const char* label, float* val, float drag_speed, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragFloat(label, val, drag_speed, 0.f, 0.f, format.value_or("%.3f"), flags);
}
inline auto imgui_drag(const char* label, double* val, float drag_speed, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_Double, val, drag_speed, nullptr, nullptr, format.value_or("%.3f"), flags);
}

inline auto imgui_drag(const char* label, int* val, float drag_speed, int min, int max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragInt(label, val, drag_speed, min, max, format.value_or("%d"), flags);
}
inline auto imgui_drag(const char* label, float* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragFloat(label, val, drag_speed, min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_drag(const char* label, double* val, float drag_speed, double min, double max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragScalar(label, ImGuiDataType_Double, val, drag_speed, &min, &max, format.value_or("%.3f"), flags);
}
inline auto imgui_drag(const char* label, glm::vec2* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragFloat2(label, glm::value_ptr(*val), drag_speed, min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_drag(const char* label, glm::vec3* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragFloat3(label, glm::value_ptr(*val), drag_speed, min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_drag(const char* label, glm::vec4* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::DragFloat4(label, glm::value_ptr(*val), drag_speed, min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_drag(const char* label, glm::mat2* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    bool b = false;
    ImGui::PushID(val);
    glm::mat2 mat = glm::transpose(*val); // This is a simple way of having mat[0] return us a row and not a column
    b |= ImGui::DragFloat2(label, glm::value_ptr(mat[0]), drag_speed, min, max, format.value_or("%.3f"), flags);
    b |= ImGui::DragFloat2("##1", glm::value_ptr(mat[1]), drag_speed, min, max, format.value_or("%.3f"), flags);
    ImGui::PopID();
    if (b)
        *val = glm::transpose(mat);
    return b;
}
inline auto imgui_drag(const char* label, glm::mat3* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    bool b = false;
    ImGui::PushID(val);
    glm::mat3 mat = glm::transpose(*val); // This is a simple way of having mat[0] return us a row and not a column
    b |= ImGui::DragFloat3(label, glm::value_ptr(mat[0]), drag_speed, min, max, format.value_or("%.3f"), flags);
    b |= ImGui::DragFloat3("##1", glm::value_ptr(mat[1]), drag_speed, min, max, format.value_or("%.3f"), flags);
    b |= ImGui::DragFloat3("##2", glm::value_ptr(mat[2]), drag_speed, min, max, format.value_or("%.3f"), flags);
    ImGui::PopID();
    if (b)
        *val = glm::transpose(mat);
    return b;
}
inline auto imgui_drag(const char* label, glm::mat4* val, float drag_speed, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    bool b = false;
    ImGui::PushID(val);
    glm::mat4 mat = glm::transpose(*val); // This is a simple way of having mat[0] return us a row and not a column
    b |= ImGui::DragFloat4(label, glm::value_ptr(mat[0]), drag_speed, min, max, format.value_or("%.3f"), flags);
    b |= ImGui::DragFloat4("##1", glm::value_ptr(mat[1]), drag_speed, min, max, format.value_or("%.3f"), flags);
    b |= ImGui::DragFloat4("##2", glm::value_ptr(mat[2]), drag_speed, min, max, format.value_or("%.3f"), flags);
    b |= ImGui::DragFloat4("##3", glm::value_ptr(mat[3]), drag_speed, min, max, format.value_or("%.3f"), flags);
    ImGui::PopID();
    if (b)
        *val = glm::transpose(mat);
    return b;
}

inline auto imgui_slider(const char* label, int* val, int min, int max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::SliderInt(label, val, min, max, format.value_or("%d"), flags);
}
inline auto imgui_slider(const char* label, float* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::SliderFloat(label, val, min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_slider(const char* label, double* val, double min, double max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::SliderScalar(label, ImGuiDataType_Double, val, &min, &max, format.value_or("%.3f"), flags);
}
inline auto imgui_slider(const char* label, glm::vec2* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::SliderFloat2(label, glm::value_ptr(*val), min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_slider(const char* label, glm::vec3* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::SliderFloat3(label, glm::value_ptr(*val), min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_slider(const char* label, glm::vec4* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    return ImGui::SliderFloat4(label, glm::value_ptr(*val), min, max, format.value_or("%.3f"), flags);
}
inline auto imgui_slider(const char* label, glm::mat2* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    bool      b   = false;
    glm::mat2 mat = glm::transpose(*val); // This is a simple way of having mat[0] return us a row and not a column
    ImGui::PushID(val);
    b |= ImGui::SliderFloat2(label, glm::value_ptr(mat[0]), min, max, format.value_or("%.3f"), flags);
    b |= ImGui::SliderFloat2("##1", glm::value_ptr(mat[1]), min, max, format.value_or("%.3f"), flags);
    ImGui::PopID();
    if (b)
        *val = glm::transpose(mat);
    return b;
}
inline auto imgui_slider(const char* label, glm::mat3* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    bool      b   = false;
    glm::mat3 mat = glm::transpose(*val); // This is a simple way of having mat[0] return us a row and not a column
    ImGui::PushID(val);
    b |= ImGui::SliderFloat3(label, glm::value_ptr(mat[0]), min, max, format.value_or("%.3f"), flags);
    b |= ImGui::SliderFloat3("##1", glm::value_ptr(mat[1]), min, max, format.value_or("%.3f"), flags);
    b |= ImGui::SliderFloat3("##2", glm::value_ptr(mat[2]), min, max, format.value_or("%.3f"), flags);
    ImGui::PopID();
    if (b)
        *val = glm::transpose(mat);
    return b;
}
inline auto imgui_slider(const char* label, glm::mat4* val, float min, float max, std::optional<const char*> format = {}, ImGuiSliderFlags flags = 0) -> bool
{
    bool      b   = false;
    glm::mat4 mat = glm::transpose(*val); // This is a simple way of having mat[0] return us a row and not a column
    ImGui::PushID(val);
    b |= ImGui::SliderFloat4(label, glm::value_ptr(mat[0]), min, max, format.value_or("%.3f"), flags);
    b |= ImGui::SliderFloat4("##1", glm::value_ptr(mat[1]), min, max, format.value_or("%.3f"), flags);
    b |= ImGui::SliderFloat4("##2", glm::value_ptr(mat[2]), min, max, format.value_or("%.3f"), flags);
    b |= ImGui::SliderFloat4("##3", glm::value_ptr(mat[3]), min, max, format.value_or("%.3f"), flags);
    ImGui::PopID();
    if (b)
        *val = glm::transpose(mat);
    return b;
}

template<typename T>
static constexpr auto biggest_number() -> T;

template<>
constexpr auto biggest_number<float>() -> float
{
    return FLT_MAX;
}
template<>
constexpr auto biggest_number<double>() -> double
{
    return DBL_MAX;
}

template<>
constexpr auto biggest_number<int>() -> int
{
    return INT_MAX - 1; // - 1 to make sure it won't overflow even when we flip the sign.
}

/// E.g. ExactType is glm::vec3 and BaseType is float.
template<typename ExactType, typename BaseType>
auto imgui_widget(const char* label, ExactType* value, BoundsMetadata<BaseType> const& bounds, std::optional<const char*> format = {}) -> bool
{
    auto const flags = ImGuiSliderFlags_NoRoundToFormat
                       | (bounds.is_logarithmic ? ImGuiSliderFlags_Logarithmic : 0);
    if (bounds.use_slider)
    {
        return imgui_slider(
            label,
            value,
            bounds.min,
            bounds.max,
            format,
            flags
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return imgui_drag(
            label,
            value,
            bounds.drag_speed,
            bounds.has_min_bound ? bounds.min : -biggest_number<BaseType>(),
            bounds.has_max_bound ? bounds.max : +biggest_number<BaseType>(),
            format,
            flags
        );
    }
}

template<typename T>
auto imgui_min_max_bounds(T& min, T& max, bool& has_min_bound, bool& has_max_bound, float& drag_speed) -> bool
{
    bool b = false;

    b |= ImGui::Checkbox("##1", &has_min_bound);
    ImGui::SameLine();

    if (has_min_bound)
    {
        b |= imgui_drag("##2", &min, 0.01f);
    }
    else
    {
        ImGuiExtras::disabled_if(true, "Toggle the checkbox on the left to set a min value.", [&]() {
            b |= imgui_drag("##3", &min, 0.01f, "-infinity");
        });
    }

    ImGui::SameLine();
    ImGui::TextDisabled("to");
    ImGui::SameLine();

    b |= ImGui::Checkbox("##4", &has_max_bound);
    ImGui::SameLine();

    if (has_max_bound)
    {
        b |= imgui_drag("##5", &max, 0.01f);
    }
    else
    {
        ImGuiExtras::disabled_if(true, "Toggle the checkbox on the left to set a max value.", [&]() {
            b |= imgui_drag("##6", &max, 0.01f, "+infinity");
        });
    }

    b |= ImGui::DragFloat("Drag speed", &drag_speed, 0.0001f, 0.000001f, biggest_number<float>(), "%.6f");

    return b;
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
        b |= imgui_min_max_bounds(meta.min, meta.max, meta.has_min_bound, meta.has_max_bound, meta.drag_speed);
    }

    b |= ImGuiExtras::toggle("Use a slider", &meta.use_slider);
    b |= ImGuiExtras::toggle("Is logarithmic", &meta.is_logarithmic);

    return b;
}

} // namespace Cool::internal