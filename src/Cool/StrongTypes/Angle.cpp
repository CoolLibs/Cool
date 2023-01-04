#include "Angle.h"
#include "imgui.h"

namespace Cool {

auto Angle::imgui_widget(std::string_view name, int number_of_snaps, float snaps_offset) -> bool
{
    bool b = false;
    ImGui::PushID(name.data());

    b |= ImGuiExtras::angle_wheel("", &value.value, number_of_snaps, snaps_offset);

    ImGui::SameLine();

    b |= ImGuiExtras::angle_slider(name.data(), &value.value);

    ImGui::PopID();
    return b;
}

} // namespace Cool