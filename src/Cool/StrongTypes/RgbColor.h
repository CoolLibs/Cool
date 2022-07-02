#pragma once

#include <op/op.hpp>

namespace Cool {
struct RgbColor
    : public op::Addable<RgbColor>
    , public op::Subtractable<RgbColor>
    , public op::Multipliable<RgbColor>
    , public op::Scalable<RgbColor>
    , public op::EqualityComparable<RgbColor> {
    glm::vec3 value{};
    constexpr RgbColor() = default; // Constructors are not implcitly created by the compiler because we inherit from some stuff
    constexpr explicit RgbColor(const glm::vec3& value)
        : value{value}
    {}

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("RGB", value));
    }
};

inline auto to_string(const Cool::RgbColor& color) -> std::string
{
    return glm::to_string(color.value);
}

inline auto color_widget(const std::string& name, glm::vec3& color, ImGuiColorEditFlags flags) -> bool
{
    return ImGui::ColorEdit3(
        name.c_str(),
        glm::value_ptr(color),
        flags
    );
}

} // namespace Cool
