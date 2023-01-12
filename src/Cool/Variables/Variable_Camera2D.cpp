#include "Variable_Camera2D.h"
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool {

auto imgui_widget(Variable<Camera2D>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value
    );
}

auto imgui_widget(VariableMetadata<Camera2D>&) -> bool
{
    return false;
}

} // namespace Cool