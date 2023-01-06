#include "Point2D.h"
#include <stringify/stringify.hpp>

namespace Cool {

auto to_string(Point2D point2D) -> std::string
{
    return Cool::stringify(point2D.value);
}

auto imgui_widget(std::string_view name, Point2D& point2D, float drag_speed) -> bool
{
    return ImGui::DragFloat2(name.data(), glm::value_ptr(point2D.value), drag_speed);
}

} // namespace Cool
