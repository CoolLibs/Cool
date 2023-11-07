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
    b |= imgui_enable_multi_viewport();
    ImGui::NewLine();
    ImGuiExtras::separator_text("Advanced");
    b |= imgui_nodes_developer_mode();

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
    ImGui::SetItemTooltip("%s", "Adds additional icons for some menus, buttons, etc.");
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

auto UserSettings::imgui_enable_multi_viewport() -> bool
{
    bool const b = ImGuiExtras::toggle("Enable multi-viewport", &enable_multi_viewport);
    ImGuiExtras::help_marker(
        "When enabled, allows you to drag windows outside of the main window."
#if defined(__linux__)
        "\nNote that on Linux this can cause issues with context menus if you use a custom window manager."
#endif
    );
    if (b)
        apply_multi_viewport_setting();
    return b;
}

void UserSettings::apply_multi_viewport_setting() const
{
#if defined(COOL_UPDATE_APP_ON_SEPARATE_THREAD)
    return; // Platform windows freeze if we are not rendering on the main thread (TODO(JF) : need to investigate that bug ; it is probably coming directly from ImGui)
#endif
    if (enable_multi_viewport)
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    else
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
}

auto UserSettings::imgui_nodes_developer_mode() -> bool
{
    bool const b = ImGuiExtras::toggle("Nodes developer mode", &nodes_developer_mode);
    ImGuiExtras::help_marker("If you want to write your own nodes, enable this. It will enable hot reloading of the nodes files.");
    return b;
}

auto should_enable_multi_viewport_by_default() -> bool
{
#if !defined(__linux__)
    return true;
#else
    // On Linux this can conflict with tiling WM and make our context menus behave weirdly.
    // https://cdn.discordapp.com/attachments/848704719987671070/1127711921651597332/ui-linux.mp4
    // https://github.com/ocornut/imgui/issues/2117
    // For example i3 has issues with it.
    // The desktops listed here should be okay though: https://wiki.archlinux.org/title/Xdg-utils#Environment_variables
    char const* var = std::getenv("XDG_CURRENT_DESKTOP");
    if (!var)
        return false;
    auto const vari = std::string{var};
    return vari == "Cinnamon"
           || vari == "X-Cinnamon"
           || vari == "Deepin"
           || vari == "DEEPIN"
           || vari == "deepin"
           || vari == "ENLIGHTENMENT"
           || vari == "GNOME"
           || vari == "GNOME-Flashback"
           || vari == "GNOME-Flashback:GNOME"
           || vari == "KDE"
           || vari == "LXDE"
           || vari == "LXQt"
           || vari == "MATE"
           || vari == "XFCE";
#endif
}

} // namespace Cool
