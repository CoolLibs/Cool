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
    ImGuiExtras::ImGradientWidget value{};
    constexpr Gradient() = default; // Constructors are not implcitly created by the compiler because we inherit from some stuff
    explicit Gradient(const ImGuiExtras::ImGradientWidget& value)
        : value{value}
    {
    }

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
    return "Gradient [...]";
}

inline auto gradient_widget(Cool::Gradient& gradient) -> bool
{
    static ImGuiExtras::ImGradientWidget gradient_wid{};
    // TO DO(ASG) alpha when removing all
    // TO DO(ASG) better fix for out of screen
    // TO DO(ASG) dragging mark selected when created
    // TO DO(ASG) open a popup when created mark
    // TO DO(ASG) when moving re-order
    return gradient_wid.gradient_editor();
}

} // namespace Cool