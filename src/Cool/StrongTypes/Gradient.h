#pragma once

#include <Cool/Math/constants.h>
#include <Cool/Serialization/ImGuiSerialization.h>
#include <cereal/types/list.hpp>
#include <imgui_gradient/imgui_gradient.hpp>
#include <op/op.hpp>
#include "RgbColor.h"

namespace Cool {
struct Gradient
    : public op::Addable<Gradient>
    , public op::Subtractable<Gradient> {
    ImGG::GradientWidget value{};
    constexpr Gradient() = default; // Constructors are not implicitly created by the compiler because we inherit from some stuff
    explicit Gradient(const ImGG::GradientWidget& value)
        : value{value}
    {
    }

    friend auto operator==(const Gradient& a, const Gradient& b) -> bool { return a.value.gradient() == b.value.gradient(); }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Gradient", value.gradient().get_marks())
        );
    }
    friend class cereal::access;
    template<class Archive>
    void load(Archive& archive)
    {
        std::list<ImGG::Mark> gradient_loaded{};
        archive(gradient_loaded);
        value.gradient() = ImGG::Gradient{gradient_loaded};
    }
};

inline auto to_string(Cool::Gradient) -> std::string
{
    return "Gradient [...]";
}

inline auto gradient_widget(std::string_view name, Cool::Gradient& gradient, ImGuiColorEditFlags flags) -> bool
{
    ImGG::Settings settings{};
    settings.color_edit_flags = flags;
    settings.flags            = ImGG::Flag::NoResetButton;
    return gradient.value.widget(name.data(), settings);
}

} // namespace Cool

namespace cereal {

template<class Archive>
void serialize(Archive& archive, ImGG::Mark& mark)
{
    archive(
        cereal::make_nvp("Mark position", mark.position),
        cereal::make_nvp("Mark color", mark.color)
    );
}

template<class Archive>
void save(Archive& archive, ImGG::RelativePosition const& position)
{
    archive(cereal::make_nvp("Relative position", position.get()));
}

template<class Archive>
void load(Archive& archive, ImGG::RelativePosition& position)
{
    float position_loaded{};
    archive(position_loaded);
    position = ImGG::RelativePosition{position_loaded};
}

} // namespace cereal