#include "WebcamsInfos.h"
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <webcam_info/webcam_info.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "WebcamsConfigs.h"

namespace Cool {

WebcamsInfos::~WebcamsInfos()
{
    _wants_to_stop_thread.store(true);
    _thread.join();
}

auto WebcamsInfos::instance() -> WebcamsInfos&
{
    static auto inst = WebcamsInfos{};
    return inst;
}

void WebcamsInfos::thread_job(WebcamsInfos& This)
{
    std::vector<webcam_info::Info> wip_webcams_infos{};
    while (!This._wants_to_stop_thread.load())
    {
        wip_webcams_infos = webcam_info::grab_all_webcams_infos();
        {
            std::scoped_lock lock{This._mutex};
            std::swap(wip_webcams_infos, This._webcams_infos);
        }
    }
}

auto WebcamsInfos::index(std::string const& webcam_name) -> std::optional<size_t>
{
    std::scoped_lock lock{_mutex};
    for (size_t i = 0; i < _webcams_infos.size(); i++)
    {
        if (_webcams_infos[i].name == webcam_name)
            return i;
    }
    return std::nullopt;
}

auto WebcamsInfos::name(size_t index) -> std::optional<std::string>
{
    std::scoped_lock lock{_mutex};
    if (index >= _webcams_infos.size())
        return std::nullopt;
    return _webcams_infos[index].name;
}

auto WebcamsInfos::imgui_widget_webcam_name(std::string& webcam_name) -> bool
{
    if (webcam_name.empty()) // HACK to make sure a newly created webcam variable has a valid webcam name.
        webcam_name = default_webcam_name();

    if (ImGuiExtras::button_with_text_icon(ICOMOON_COG))
        WebcamsConfigs::instance().open_imgui_window();
    ImGui::SetItemTooltip("%s", "Open webcam config to choose its resolution. This can help if the refresh rate of your webcam is too slow.");

    bool       b                  = false;
    auto const combo_preview_name = webcam_name;
    if (ImGui::BeginCombo("Webcam", combo_preview_name.c_str()))
    {
        std::scoped_lock lock{_mutex};
        for (auto const& info : _webcams_infos)
        {
            bool const is_selected = webcam_name == info.name;
            if (ImGui::Selectable(info.name.c_str(), is_selected))
            {
                b           = true;
                webcam_name = info.name;
            }
            if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    return b;
}

auto WebcamsInfos::default_resolution(std::string const& webcam_name, bool do_lock) -> webcam_info::Resolution
{
    return do_lock ? default_resolution_locking(webcam_name) : default_resolution_non_locking(webcam_name);
}

auto WebcamsInfos::default_resolution_locking(std::string const& webcam_name) -> webcam_info::Resolution
{
    std::scoped_lock lock{_mutex};
    return default_resolution_non_locking(webcam_name);
}

auto WebcamsInfos::default_resolution_non_locking(std::string const& webcam_name) -> webcam_info::Resolution
{
    for (auto const& infos : _webcams_infos)
    {
        if (infos.name != webcam_name)
            continue;

        if (infos.available_resolutions.empty())
            return {};
        return infos.available_resolutions[0];
    }

    return {};
}

auto WebcamsInfos::default_webcam_name() -> std::string
{
    std::scoped_lock lock{_mutex};

    return _webcams_infos.empty() ? "" : _webcams_infos[0].name;
}

void WebcamsInfos::for_each_webcam_info(std::function<void(webcam_info::Info const&)> const& callback)
{
    std::scoped_lock lock{_mutex};
    for (auto const& info : _webcams_infos)
        callback(info);
}

} // namespace Cool