#include "WebcamInfo.h"
#include <imgui.h>
#include <charconv>
#include <string>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

auto WebcamInfo::imgui_widget(const int number_of_webcam) -> bool
{
    bool b = false;

    std::string text;
    for (int i = 0; i < number_of_webcam; i++)
    {
        char val = static_cast<char>(i);
        text.append("0" + i);
        text.push_back('\0');
    }

    b |= ImGui::Combo("Camera Displayed", &webcam_index, text.c_str());

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