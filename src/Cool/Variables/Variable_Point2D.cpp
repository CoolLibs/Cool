#include "Variable_Point2D.h"

namespace Cool {

auto imgui_widget(Variable<Point2D>& var) -> bool
{
    return imgui_widget(
        var.name.c_str(),
        var.value
    );
}

auto imgui_widget(VariableMetadata<Point2D>&) -> bool
{
    return false;
}

} // namespace Cool