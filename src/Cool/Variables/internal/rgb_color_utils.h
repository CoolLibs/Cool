#pragma once
#include <Cool/ImGui/ImGuiExtras.h>

namespace Cool::internal {

inline auto color_imgui_flags(bool is_hdr) -> ImGuiColorEditFlags
{
    return ImGuiColorEditFlags_Float
           | (is_hdr ? ImGuiColorEditFlags_HDR : 0)
           | ImGuiColorEditFlags_PickerHueWheel
           | ImGuiColorEditFlags_AlphaBar;
}

inline auto rgb_color_metadata_widget(bool& is_hdr) -> bool
{
    const bool b = ImGui::Checkbox("HDR", &is_hdr);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows the RGB values to go outside of the [0, 1] range.");
    return b;
}

inline auto rgba_color_metadata_widget(bool& is_hdr) -> bool
{
    const bool b = ImGui::Checkbox("HDR", &is_hdr);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows the RGBA values to go outside of the [0, 1] range.");
    return b;
}

} // namespace Cool::internal