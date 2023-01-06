#include "Color.h"
#include "Cool/ColorSpaces/conversions.h"

namespace Cool {

auto Color::from_srgb(glm::vec3 const& srgb) -> Color
{
    Color res{};
    res._srgb = srgb;
    return res;
}

#include "Cool/ColorSpaces/generated/define_color_getters.inl"

auto to_string(Color const& color) -> std::string
{
    return fmt::format("{} (sRGB)", glm::to_string(color.as_sRGB()));
}

auto imgui_widget(std::string_view name, Color& color, ImGuiColorEditFlags flags) -> bool
{
    glm::vec3 srgb = color.as_sRGB();
    if (ImGui::ColorEdit3(
            name.data(),
            glm::value_ptr(srgb),
            flags
        ))
    {
        color = Color::from_srgb(srgb);
        return true;
    }
    return false;
}

} // namespace Cool