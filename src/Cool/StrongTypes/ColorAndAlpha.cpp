#include "ColorAndAlpha.h"
#include "Cool/ColorSpaces/conversions.h"
#include "Cool/StrongTypes/ColorAndAlpha.h"

namespace Cool {

auto ColorAndAlpha::from_srgb_straight_alpha(glm::vec4 const& srgb_straight) -> ColorAndAlpha
{
    ColorAndAlpha res{};
    res._srgb_straight = srgb_straight;
    return res;
}

#include "Cool/ColorSpaces/generated/define_color_and_alpha_getters.inl"

auto to_string(ColorAndAlpha const& color) -> std::string
{
    return fmt::format("{} (sRGB, Straight alpha)", glm::to_string(color.as_sRGB_StraightA()));
}

auto imgui_widget(std::string_view name, ColorAndAlpha& color, ImGuiColorEditFlags flags) -> bool
{
    glm::vec4 srgb_straight = color.as_sRGB_StraightA();
    if (ImGui::ColorEdit4(
            name.data(),
            glm::value_ptr(srgb_straight),
            flags
        ))
    {
        color = ColorAndAlpha::from_srgb_straight_alpha(srgb_straight);
        return true;
    }
    return false;
}

} // namespace Cool