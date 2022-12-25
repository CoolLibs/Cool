#pragma once

#include <op/op.hpp>

namespace Cool {

class PremultipliedRgbaColor
    : public op::Addable<PremultipliedRgbaColor>
    , public op::Subtractable<PremultipliedRgbaColor>
    , public op::Multipliable<PremultipliedRgbaColor>
    , public op::Scalable<PremultipliedRgbaColor> {
public:
    constexpr PremultipliedRgbaColor() = default; // Constructors are not implicitly created by the compiler because we inherit from some stuff
    constexpr explicit PremultipliedRgbaColor(glm::vec4 const& rgba)
        : value{rgba}
    {}

    [[nodiscard]] auto get() const -> glm::vec4
    {
        glm::vec3 const linear_rgb = glm::pow(glm::vec3{value.r, value.g, value.b}, glm::vec3{2.2f});
        return glm::vec4{
            linear_rgb.r * value.a,
            linear_rgb.g * value.a,
            linear_rgb.b * value.a,
            value.a,
        };
    }

    friend auto operator==(PremultipliedRgbaColor const& a, PremultipliedRgbaColor const& b) -> bool { return a.value == b.value; }

private:
    glm::vec4 value{}; // Stored as straight alpha because this is what the UI expects.

    friend auto imgui_color_widget(std::string_view, PremultipliedRgbaColor&, ImGuiColorEditFlags) -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("RGBA (Premultiplied)", value));
    }
};

inline auto to_string(Cool::PremultipliedRgbaColor const& color) -> std::string
{
    return fmt::format("{} (Premultiplied)", glm::to_string(color.get()));
}

inline auto imgui_color_widget(std::string_view name, PremultipliedRgbaColor& color, ImGuiColorEditFlags flags) -> bool
{
    return ImGui::ColorEdit4(
        name.data(),
        glm::value_ptr(color.value),
        flags
    );
}

} // namespace Cool
