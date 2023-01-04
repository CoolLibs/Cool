#include "Angle.h"
#include "imgui.h"

namespace Cool {

auto Angle::imgui_widget(std::string_view name, int number_of_snaps, float snaps_offset, bool always_snap) -> bool
{
    bool b = false;
    ImGui::PushID(name.data());

    b |= ImGuiExtras::angle_wheel(name.data(), &value.value, number_of_snaps, snaps_offset, always_snap);

    ImGui::SameLine();

    b |= ImGuiExtras::angle_slider("", &value.value);

    ImGui::PopID();
    return b;
}

} // namespace Cool