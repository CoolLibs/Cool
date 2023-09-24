#include "Variable_MidiChannel.h"

namespace Cool {

auto imgui_widget(Variable<MidiChannel>& var) -> bool
{
    return ImGui::InputInt(var.name().c_str(), &var.value().index, 1, 1);
}

auto imgui_widget(VariableMetadata<MidiChannel>&) -> bool
{
    return false;
}

} // namespace Cool