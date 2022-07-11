#pragma once

#include <Cool/ImGui/imgui_color_gradient.h>
#include <Cool/Math/constants.h>
#include <op/op.hpp>
#include "RgbColor.h"

namespace Cool {
struct Gradient
    : public op::Addable<Gradient>
    , public op::Subtractable<Gradient>
    , public op::EqualityComparable<Gradient> {
    ImGradient value{};
    // float value{};
    constexpr Gradient() = default; // Constructors are not implcitly created by the compiler because we inherit from some stuff
    explicit Gradient(const ImGradient& value)
        : value{value}
    {
    }
    //     constexpr explicit Gradient(float value)
    //     : value{value}
    // {
    // }
    // auto inigo_formula(const glm::vec3 a, const glm::vec3 b, const glm::vec3 c, const glm::vec3 d) -> Cool::RgbColor
    // {
    //     return Cool::RgbColor(a + b * glm::cos(tau * (c * value + d)));
    // }
    // auto get_color() -> Cool::RgbColor
    // {
    //     const glm::vec3 a = glm::vec3(0.5, 0.5, 0.5);
    //     const glm::vec3 b = glm::vec3(0.5, 0.5, 0.5);
    //     const glm::vec3 c = glm::vec3(1., 1., 1.);
    //     const glm::vec3 d = glm::vec3(0., 0.33, 0.67);
    //     return inigo_formula(a, b, c, d);
    // }
    // void set_color(const float position) { value = position; }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Gradient", value));
    }
};

inline auto to_string(Cool::Gradient gradient) -> std::string
{
    // return to_string(gradient.get_color());
    float* color = nullptr;
    gradient.value.getColorAt(0.5f, color);
    return std::to_string(*color);
}

inline auto gradient_widget(Cool::Gradient& gradient) -> bool
{
    // ImGradientMark* draggingMark = nullptr;
    // // draggingMark->position       = 0.1f;
    // // draggingMark->color    = {1.f, 0.f, 0.f, 0.f};
    // ImGradientMark* selectedMark = nullptr;
    // selectedMark->position       = 0.9f;
    // selectedMark->color    = {0.f, 1.f, 0.f, 0.f};
    // if(ImGui::IsMouseDragging(ImGuiPopupFlags_MouseButtonRight)){
    //     ImVec2 p      = ImGui::GetCursorScreenPos();
    //     dragginMark = gradient.value.getMarks()[]
    // }
    // bool updated = false;
    // if (ImGui::GradientButton(&gradient.value))
    // {
    // set show editor flag to true/false
    static ImGradientMark* draggingMark = nullptr;
    static ImGradientMark* selectedMark = nullptr;

    bool updated = ImGui::GradientEditor(&gradient.value, draggingMark, selectedMark);
    // }
    // if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
    // {
    //     gradient.value.removeMark(draggingMark);
    // }
    if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
    {
        gradient.value.addMark(selectedMark->position, ImColor(selectedMark->color[0], selectedMark->color[1], selectedMark->color[2], selectedMark->color[3]));
    }

    return updated;
}

} // namespace Cool