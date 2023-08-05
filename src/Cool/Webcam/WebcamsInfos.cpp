#include "WebcamsInfos.h"
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <webcam_info/webcam_info.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Webcam/WebcamsConfigs.h"

namespace Cool {

auto WebcamsInfos::instance() -> WebcamsInfos&
{
    static auto inst = WebcamsInfos{};
    return inst;
}

void WebcamsInfos::thread_webcams_infos_works(std::stop_token const& stop_token, WebcamsInfos& This)
{
    std::vector<webcam_info::Info> wip_webcams_infos{};
    while (!stop_token.stop_requested())
    {
        wip_webcams_infos = webcam_info::grab_all_webcams_infos();
        {
            std::scoped_lock<std::mutex> lock(This._mutex_webcam_info);
            std::swap(wip_webcams_infos, This._webcams_infos);
        }
    }
}

auto WebcamsInfos::get_index_from_name(const std::string name) -> std::optional<size_t>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    for (size_t i = 0; i < _webcams_infos.size(); i++)
    {
        if (_webcams_infos[i].name == name)
            return i;
    }
    return std::nullopt;
}

auto WebcamsInfos::get_name_from_index(size_t index) -> std::optional<std::string>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    if (index >= _webcams_infos.size())
        return std::nullopt;
    return _webcams_infos[index].name;
}

auto WebcamsInfos::imgui_widget_webcam_name(std::string& webcam_name) -> bool
{
    // TODO need to lock ? Can use for_each ?    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    if (webcam_name.empty()) // HACK to make sure a newly created webcam variable has a valid webcam name.
        webcam_name = get_default_webcam_name();
    bool b = false;

    std::vector<std::string> list_name{};
    list_name.reserve(_webcams_infos.size());
    {
        std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
        for (auto const& infos : _webcams_infos)
        {
            list_name.push_back(infos.name);
        }
    }

    if (ImGuiExtras::button_with_text_icon(ICOMOON_COG))
        WebcamsConfigs::instance().open_webcams_config_window();

    ImGuiExtras::tooltip("Open Webcams Config to choose the resolutions");
    // ImGui::SameLine();

    std::string combo_preview_name = webcam_name;

    if (ImGui::BeginCombo("Webcam", combo_preview_name.c_str(), 0))
    {
        for (auto& name : list_name)
        {
            const bool is_selected = (webcam_name == name);
            if (ImGui::Selectable(name.c_str(), is_selected))
                webcam_name = name;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return b;
}

auto WebcamsInfos::get_default_resolution_from_name(const std::string& name) -> std::optional<webcam_info::Resolution>
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);

    for (auto& infos : _webcams_infos)
    {
        if (infos.name == name)
        {
            if (infos.available_resolutions.empty())
                return std::nullopt;
            return infos.available_resolutions[0];
        }
    }
    return std::nullopt;
}

auto WebcamsInfos::get_default_webcam_name() -> std::string
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);

    return _webcams_infos.empty() ? "" : _webcams_infos[0].name;
}

void WebcamsInfos::for_each_webcam_info(std::function<void(webcam_info::Info const&)> const& callback)
{
    std::scoped_lock<std::mutex> lock(_mutex_webcam_info);
    for (auto& info : _webcams_infos)
        callback(info);
}

} // namespace Cool