#include "ImGuiExtrasStyle.h"
#include <imgui.h>
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool::ImGuiExtras {

auto Style::imgui() -> bool
{
    bool b = false;

    ImGui::SliderFloat("Floating buttons spacing", &floating_buttons_spacing, 0.f, 20.f);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Small buttons that overlap the View");

    return b;
}

} // namespace Cool::ImGuiExtras