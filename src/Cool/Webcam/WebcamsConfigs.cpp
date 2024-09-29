#include "WebcamsConfigs.hpp"
#include <wcam/wcam.hpp>
#include "Cool/internal/Serialization/SWebcamsConfigs.h"

namespace Cool {

auto WebcamsConfigs::gen_instance() -> WebcamsConfigs&
{
    static auto inst = WebcamsConfigs{};
    // auto const  maybe_err = do_load(inst._configs, inst._serializer);
    // std::ignore           = maybe_err; // Ignore errors when file not found
    return inst;
}

auto WebcamsConfigs::instance() -> WebcamsConfigs&
{
    static auto& inst = gen_instance(); // NOLINT(*avoid-non-const-global-variables)
    return inst;
}

void WebcamsConfigs::open_imgui_window()
{
    _window.open();
}

void WebcamsConfigs::imgui_window()
{
    _window.show([&](bool /*is_opening*/) {
        auto const webcam_infos = wcam::all_webcams_info();
        int        imgui_id{0};
        for (auto const& info : webcam_infos)
        {
            ImGui::PushID(imgui_id++);

            ImGui::SeparatorText(info.name.c_str());
            auto const selected_resolution = wcam::get_selected_resolution(info.id);
            if (ImGui::BeginCombo("Resolution", wcam::to_string(selected_resolution).c_str()))
            {
                for (auto const& resolution : info.resolutions)
                {
                    bool const is_selected = resolution == selected_resolution;
                    if (ImGui::Selectable(wcam::to_string(resolution).c_str(), is_selected))
                        wcam::set_selected_resolution(info.id, resolution);

                    if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::PopID();
            ImGui::NewLine();
        }
    });
}

} // namespace Cool