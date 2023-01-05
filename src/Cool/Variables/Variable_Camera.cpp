#include "Variable_Camera.h"

namespace Cool {

auto imgui_widget(Variable<Cool::Camera>&) -> bool
{
    return false;
}

auto imgui_widget(VariableMetadata<Cool::Camera>&) -> bool
{
    return false;
}

} // namespace Cool