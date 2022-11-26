#pragma once

#include <op/op.hpp>

namespace Cool {

struct StraightRgbaColor
    : public op::Addable<StraightRgbaColor>
    , public op::Subtractable<StraightRgbaColor>
    , public op::Multipliable<StraightRgbaColor>
    , public op::Scalable<StraightRgbaColor>
    , public op::EqualityComparable<StraightRgbaColor> {
    glm::vec4 value{};

    constexpr StraightRgbaColor() = default; // Constructors are not implicitly created by the compiler because we inherit from some stuff
    constexpr explicit StraightRgbaColor(glm::vec4 const& rgba)
        : value{rgba}
    {}

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("RGBA (Straight)", value));
    }
};

inline auto to_string(Cool::StraightRgbaColor const& color) -> std::string
{
    return fmt::format("{} (Straight)", glm::to_string(color.value));
}

inline auto imgui_color_widget(std::string_view name, StraightRgbaColor& color, ImGuiColorEditFlags flags) -> bool
{
    return ImGui::ColorEdit4(
        name.data(),
        glm::value_ptr(color.value),
        flags
    );
}

} // namespace Cool
