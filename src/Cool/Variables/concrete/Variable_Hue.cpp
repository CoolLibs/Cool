#include "Variable_Hue.h"

namespace Cool {

auto imgui_widget(Variable<Hue>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value
    );
}

auto imgui_widget(Metadata<Hue>&) -> bool
{
    return false;
}

} // namespace Cool