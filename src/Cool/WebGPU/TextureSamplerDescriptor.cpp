#include "TextureSamplerDescriptor.hpp"

namespace Cool {

auto TextureSamplerDescriptor::imgui_widget() -> bool
{
    bool b = false;
    b |= ImGui::Combo("Repeat Mode", reinterpret_cast<int*>(&repeat_mode), "None\0Mirror\0Mosaic\0Clamp\0\0");

    int mode = interpolation_mode == InterpolationMode::NearestNeighbour ? 0 : 1;
    if (ImGui::Combo("Interpolation Mode", &mode, "Nearest Neighbour\0Linear\0\0"))
    {
        interpolation_mode = mode == 0
                                 ? InterpolationMode::NearestNeighbour
                                 : InterpolationMode::Linear;

        b = true;
    }
    return b;
}

} // namespace Cool