#include "UserSettings.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include <smart/smart.hpp>
#include "imgui.h"

namespace Cool {

auto UserSettings::imgui() -> bool
{
    bool b = false;
    ImGuiExtras::separator_text("Autosave");
    b |= imgui_autosave();
    ImGuiExtras::separator_text("UI");
    b |= imgui_extra_icons();
    ImGuiExtras::separator_text("Camera");
    b |= imgui_camera2D_zoom_sensitivity();
    ImGuiExtras::separator_text("Miscellaneous");
    b |= imgui_single_click_to_input_in_drag_widgets();

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

auto UserSettings::imgui_single_click_to_input_in_drag_widgets() -> bool
{
    bool const b = ImGuiExtras::toggle("Single-click to input in \"Drag number\" widgets.", &single_click_to_input_in_drag_widgets);
    ImGuiExtras::help_marker("When disabled, you need to double-click or CTRL+click on a \"Drag number\" widget to be able to write down a specific value.");
    return b;
}

} // namespace Cool
