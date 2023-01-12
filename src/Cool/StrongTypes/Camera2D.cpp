#include "Camera2D.h"

namespace Cool {

auto Camera2D::transform_matrix() const -> glm::mat3
{
    return {};
}
auto to_string(Camera2D const&) -> std::string
{
    return {};
}
auto imgui_widget(std::string_view name, Camera2D&) -> bool
{
    return {};
}

} // namespace Cool