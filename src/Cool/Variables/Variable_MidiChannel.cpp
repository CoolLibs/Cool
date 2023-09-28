#include "Variable_MidiChannel.h"
#include <imgui.h>
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool {

auto imgui_widget(Variable<MidiChannel>& var) -> bool
{
    if (ImGuiExtras::button_with_text_icon(ICOMOON_COG))
    {
        // TODO(Midi) Open the Midi Config window
    }
    ImGui::SetItemTooltip("%s", "Open Midi Config to select the controller in use and verify we receive messages from it.");

    bool const b = ImGui::InputScalar(var.name().c_str(), ImGuiDataType_S32, &var.value().index);

    return b;
}

auto imgui_widget(VariableMetadata<MidiChannel>&) -> bool
{
    return false;
}

} // namespace Cool