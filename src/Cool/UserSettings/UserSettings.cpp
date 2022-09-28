#include "UserSettings.h"
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool {

auto UserSettings::imgui_autosave() -> bool
{
    return ImGuiExtras::checkbox_with_submenu("Autosave", &autosave_enabled, [&]() {
        return ImGui::InputFloat("Delay (in seconds)", &autosave_delay_in_seconds);
    });
}

auto UserSettings::imgui() -> bool
{
    bool b = false;
    b |= imgui_autosave();

    return b;
}

} // namespace Cool
