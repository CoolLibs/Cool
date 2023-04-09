#include "UserSettings.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <smart/smart.hpp>
#include "imgui.h"

namespace Cool {

auto UserSettings::imgui() -> bool
{
    bool b = false;
    ImGui::SeparatorText("Autosave");
    b |= imgui_autosave();
    ImGui::SeparatorText("UI");
    b |= imgui_extra_icons();
    ImGui::SeparatorText("Camera");
    b |= imgui_camera2D_zoom_sensitivity();

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

auto UserSettings::imgui_extra_icons() -> bool
{
    bool const b = ImGuiExtras::toggle("Extra Icons", &extra_icons);
    ImGuiExtras::tooltip("Adds additional icons for some menus, buttons, etc.");
    return b;
}

auto UserSettings::imgui_camera2D_zoom_sensitivity() -> bool
{
    return ImGui::SliderFloat("Camera 2D zoom sensitivity", &camera2D_zoom_sensitivity, 1.0001f, 1.2f);
}

} // namespace Cool
