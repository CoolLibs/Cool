#include "Variable_Point2D.h"

namespace Cool {

auto imgui_widget(Variable<Point2D>& var) -> bool
{
    return imgui_widget(var.name().c_str(), var.value(), var.metadata().drag_speed);
}

auto imgui_widget(VariableMetadata<Point2D>& meta) -> bool
{
    return ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.0000005f, 0.000001f, FLT_MAX / static_cast<float>(INT_MAX), "%.6f");
}

} // namespace Cool