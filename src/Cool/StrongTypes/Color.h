#pragma once

namespace Cool {

class Color {
public:
    Color() = default;
    [[nodiscard]] static auto from_srgb(glm::vec3 const& srgb) -> Color;

    /// Returns a brighter version of this color.
    [[nodiscard]] auto brighter() const -> Color;

#include "Cool/ColorSpaces/generated/declare_color_getters.inl"

    [[nodiscard]] auto as_ImColor() const -> ImColor;

    friend auto operator==(Color const&, Color const&) -> bool = default;

private:
    glm::vec3 _srgb{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("sRGB", _srgb));
    }
};

auto to_string(Color const&) -> std::string;
auto imgui_widget(std::string_view name, Color&, ImGuiColorEditFlags) -> bool;

auto get_text_color(Color const& background_color) -> Color;

} // namespace Cool
