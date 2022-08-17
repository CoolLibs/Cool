#pragma once

#include <Cool/Math/constants.h>
#include <imgui_gradient/imgui_gradient.hpp>
#include <op/op.hpp>
#include "RgbColor.h"

namespace Cool {
struct Gradient
    : public op::Addable<Gradient>
    , public op::Subtractable<Gradient> {
    ImGuiGradient::GradientWidget value{};
    constexpr Gradient() = default; // Constructors are not implcitly created by the compiler because we inherit from some stuff
    explicit Gradient(const ImGuiGradient::GradientWidget& value)
        : value{value}
    {
    }

    friend auto operator==(const Gradient& a, const Gradient& b) -> bool { return a.value.gradient() == b.value.gradient(); }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Gradient", value));
    }
};

inline auto to_string(Cool::Gradient) -> std::string
{
    return "Gradient [...]";
}

inline auto gradient_widget(std::string_view name, Cool::Gradient& gradient, ImGuiColorEditFlags flags) -> bool
{
    ImGuiGradient::Settings settings{};
    settings.color_flags = flags;
    settings.flags       = ImGuiGradient::Flag::NoResetButton;
    return gradient.value.widget(name.data(), settings);
}

} // namespace Cool