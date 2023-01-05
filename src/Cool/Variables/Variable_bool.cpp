#include "Variable_bool.h"

namespace Cool {

auto imgui_widget(Variable<bool>& var) -> bool
{
    return ImGui::Checkbox(var.name.c_str(), &var.value);
}

auto imgui_widget(VariableMetadata<bool>&) -> bool
{
    return false;
}

} // namespace Cool