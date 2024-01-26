#include "Variable_OSCChannel.h"
#include <imgui.h>
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/OSC/OSCManager.h"

namespace Cool {

auto imgui_widget(Variable<OSCChannel>& var) -> bool
{
    if (ImGuiExtras::button_with_text_icon(ICOMOON_COG))
        osc_manager().open_config_window();
    ImGui::SetItemTooltip("%s", "Open OSC config to choose which port and IP address to listen to.");
    osc_manager().imgui_error_message_for_invalid_endpoint("\nUse the  " ICOMOON_COG " button above to fix this.");

    return osc_manager().imgui_channel_widget(var.name().c_str(), var.value());
}

auto imgui_widget(VariableMetadata<OSCChannel>&) -> bool
{
    return false;
}

} // namespace Cool