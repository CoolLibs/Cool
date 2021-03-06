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

    constexpr RgbColor() = default; // Constructors are not implicitly created by the compiler because we inherit from some stuff
    constexpr explicit RgbColor(const glm::vec3& rgb)
        : value{rgb}
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

inline auto imgui_color_widget(std::string_view name, RgbColor& color, ImGuiColorEditFlags flags) -> bool
{
    return ImGui::ColorEdit3(
        name.data(),
        glm::value_ptr(color.value),
        flags
    );
}

} // namespace Cool
