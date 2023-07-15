#include "Variable_MidiCc.h"

namespace Cool {

auto imgui_widget(Variable<MidiCc>& var) -> bool
{
  return  ImGui::InputInt(var.name().c_str(), &var.value().index, 1, 1);
    // TODO(Midi) faire l'input int pour choisir le slider / button sur lequel on veut se brancher
}

auto imgui_widget(VariableMetadata<MidiCc>& ) -> bool
{
    return false;
}

} // namespace Cool