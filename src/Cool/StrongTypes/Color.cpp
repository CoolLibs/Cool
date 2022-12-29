#include "Color.h"

namespace Cool {

auto Color::from_srgb(glm::vec3 const& srgb) -> Color
{
    Color res{};
    res._srgb = srgb;
    return res;
}

auto Color::as_srgb() const -> glm::vec3
{
    return _srgb;
}
auto Color::as_linear_rgb() const -> glm::vec3
{
    // TODO(JF)
    return _srgb;
}
auto Color::as_cielab() const -> glm::vec3
{
    // TODO(JF)
    return _srgb;
}

auto to_string(Color const& color) -> std::string
{
    return fmt::format("{} (sRGB)", glm::to_string(color.as_srgb()));
}

auto imgui_widget(std::string_view name, Color& color, ImGuiColorEditFlags flags) -> bool
{
    glm::vec3 srgb = color.as_srgb();
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