#include "IApp.h"

namespace Cool {

void IApp::save_windows_state()
{
    if (_imgui_ini_state_to_restore.has_value())
        return;
    _imgui_ini_state_to_restore = ImGui::SaveIniSettingsToMemory();
}

void IApp::restore_windows_state()
{
    _wants_to_restore_ini_state = true;
}

} // namespace Cool