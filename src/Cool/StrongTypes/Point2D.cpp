#pragma once

#include "Point2D.h"
#include <stringify/stringify.hpp>

namespace Cool {

auto to_string(Point2D point2D) -> std::string
{
    return Cool::stringify(point2D.value);
}

auto imgui_widget(std::string_view name, Point2D& point2D) -> bool
{
    return ImGui::DragFloat2(name.data(), glm::value_ptr(point2D.value), 0.001f, -1.f, 1.f);
}

} // namespace Cool
