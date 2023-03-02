#include "UserSettings.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <smart/smart.hpp>
#include "Cool/ImGui/MultiViewport.h"
#include "imgui.h"

namespace Cool {

auto UserSettings::imgui() -> bool
{
    bool b = false;
    b |= imgui_autosave();
    b |= imgui_camera2D_zoom_sensitivity();
    b |= imgui_enable_multiviewport();

    return b;
}

auto UserSettings::imgui_autosave() -> bool
{
    return ImGuiExtras::checkbox_with_submenu("Autosave", &autosave_enabled, [&]() {
        bool b                    = ImGui::InputFloat("Delay (in seconds)", &autosave_delay_in_seconds);
        autosave_delay_in_seconds = smart::keep_above(1.f, autosave_delay_in_seconds); // Make sure the delay is at least 1 second

        return b;
    });
}

auto UserSettings::imgui_camera2D_zoom_sensitivity() -> bool
{
    return ImGui::SliderFloat("Camera 2D zoom sensitivity", &camera2D_zoom_sensitivity, 1.0001f, 1.2f);
}

auto UserSettings::imgui_enable_multiviewport() -> bool
{
    bool const b = ImGui::Checkbox("Enable multi-windows", &enable_multiviewport);
    if (b)
    {
        if (enable_multiviewport)
            enable_imgui_multiviewport();
        else
            disable_imgui_multiviewport();
    }
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows you to drag sub-windows outside of the main window. Disabling this can fix the UI visual glitches that appear on some computers.");
    return b;
}

} // namespace Cool
