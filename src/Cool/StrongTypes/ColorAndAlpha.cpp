#include "ColorAndAlpha.h"
#include "Cool/StrongTypes/ColorAndAlpha.h"

namespace Cool {

auto ColorAndAlpha::from_srgb_straight_alpha(glm::vec4 const& srgb_straight) -> ColorAndAlpha
{
    ColorAndAlpha res{};
    res._srgb_straight = srgb_straight;
    return res;
}

auto ColorAndAlpha::as_srgb_straight() const -> glm::vec4
{
    return _srgb_straight;
}

auto ColorAndAlpha::as_cielab_premultiplied() const -> glm::vec4
{
    // TODO(JF)
    return _srgb_straight;
}

auto to_string(ColorAndAlpha const& color) -> std::string
{
    return fmt::format("{} (sRGB, Straight alpha)", glm::to_string(color.as_srgb_straight()));
}

auto imgui_widget(std::string_view name, ColorAndAlpha& color, ImGuiColorEditFlags flags) -> bool
{
    glm::vec4 srgb_straight = color.as_srgb_straight();
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