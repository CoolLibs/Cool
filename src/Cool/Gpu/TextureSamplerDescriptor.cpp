#include "TextureSamplerDescriptor.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

auto TextureSamplerDescriptor::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGui::Combo("Repeat Mode", reinterpret_cast<int*>(&repeat_mode), "None\0Mirror\0Mosaic\0Clamp\0\0");

    int mode = interpolation_mode == Interpolation::NearestNeighbour ? 0 : 1;
    if (ImGui::Combo("Interpolation Mode", &mode, "Nearest Neighbour\0Linear\0\0"))
    {
        interpolation_mode = mode == 0
                                 ? Interpolation::NearestNeighbour
                                 : Interpolation::Linear;

        b = true;
    }
    return b;
}

} // namespace Cool