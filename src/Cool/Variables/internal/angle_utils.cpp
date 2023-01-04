#include "angle_utils.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "imgui.h"

namespace Cool::internal {

auto angle_metadata_widget(int& number_of_snaps, float& snaps_offset, bool& always_snap) -> bool
{
    bool b = false;

    b |= ImGui::InputInt("Number of snaps", &number_of_snaps, 0);
    if (number_of_snaps < 1)
        number_of_snaps = 1;

    b |= ImGuiExtras::angle_slider("Snaps offset", &snaps_offset);

    b |= ImGui::Checkbox("Always snap", &always_snap);

    return b;
}

} // namespace Cool::internal