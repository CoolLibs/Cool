#include "TextureInfo.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

auto TextureInfo::imgui_widget() -> bool
{
    bool b = false;

    b |= ImGuiExtras::file_and_folder("Image Path", &absolute_path, NfdFileFilter::Image);
    b |= ImGui::Combo("Repeat Mode", reinterpret_cast<int*>(&sampler.repeat_mode), "None\0Mirror\0Mosaic\0Clamp\0\0");

    {
        int mode = sampler.interpolation_mode == glpp::Interpolation::NearestNeighbour ? 0 : 1;
        if (ImGui::Combo("Interpolation Mode", &mode, "Nearest Neighbour\0Linear\0\0"))
        {
            sampler.interpolation_mode = mode == 0
                                             ? glpp::Interpolation::NearestNeighbour
                                             : glpp::Interpolation::Linear;

            b = true;
        }
    }

    return b;
}

} // namespace Cool