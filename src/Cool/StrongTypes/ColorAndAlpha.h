#pragma once

namespace Cool {

class ColorAndAlpha {
public:
    ColorAndAlpha() = default;
    static auto from_srgb_straight_alpha(glm::vec4 const& srgb_straight) -> ColorAndAlpha;

#include "Cool/ColorSpaces/generated/declare_color_and_alpha_getters.inl"

    friend auto operator==(ColorAndAlpha const&, ColorAndAlpha const&) -> bool = default;

private:
    glm::vec4 _srgb_straight{};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(ser20::make_nvp("sRGB, Straight alpha", _srgb_straight));
    }
};

auto to_string(ColorAndAlpha const&) -> std::string;
auto imgui_widget(std::string_view name, ColorAndAlpha&, ImGuiColorEditFlags) -> bool;

} // namespace Cool
