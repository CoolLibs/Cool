#include "Variable_bool.h"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

auto imgui_widget(Variable<bool>& var) -> bool
{
    return ImGuiExtras::toggle(var.name().c_str(), &var.value());
}

auto imgui_widget(VariableMetadata<bool>&) -> bool
{
    return false;
}

} // namespace Cool