#include "WebcamsConfigs.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <webcam_info/webcam_info.hpp>
#include "TextureLibrary_FromWebcam.h"
#include "WebcamsInfos.h"

namespace Cool {

auto WebcamsConfigs::gen_instance() -> WebcamsConfigs&
{
    static auto inst      = WebcamsConfigs{};
    auto const  maybe_err = inst._serializer.load<WebcamsConfigsList, cereal::JSONInputArchive>(inst._configs);
    std::ignore           = maybe_err; // Ignore errors when file not found
    return inst;
}

auto WebcamsConfigs::instance() -> WebcamsConfigs&
{
    static auto& inst = gen_instance(); // NOLINT(*avoid-non-const-global-variables)
    return inst;
}

auto WebcamsConfigs::selected_resolution(std::string const& webcam_name) -> webcam_info::Resolution
{
    return get_config(webcam_name).resolution;
}

auto WebcamsConfigs::get_config(std::string const& webcam_name) -> WebcamConfig&
{
    auto config = _configs.find(webcam_name);
    if (config == _configs.end())
    {
        return _configs.emplace(webcam_name, WebcamConfig{WebcamsInfos::instance().default_resolution(webcam_name)}).first->second;
    }
    return config->second;
}

void WebcamsConfigs::open_imgui_window()
{
    _window.open();
}

void WebcamsConfigs::imgui_window()
{
    _window.show([&]() {
        WebcamsInfos::instance().for_each_webcam_info([&](webcam_info::Info const& info) {
            auto& config = get_config(info.name);
            // auto        webcam_resolution   = std::make_pair(info.resolution.width, info.height);
            std::string combo_preview_value = fmt::format("{} x {}", config.resolution.width, config.resolution.height);

            if (ImGui::BeginCombo(info.name.c_str(), combo_preview_value.c_str(), 0))
            {
                for (auto& resolution : info.available_resolutions)
                {
                    const bool  is_selected     = (config.resolution == resolution);
                    std::string resolution_name = fmt::format("{} x {}", resolution.width, resolution.height);

                    if (ImGui::Selectable(resolution_name.c_str(), is_selected))
                    {
                        config.resolution = resolution;

                        TextureLibrary_FromWebcam::instance().invalidate_request(info.name); // Destroy request so that a new one will be created with the new requested resolution.

                        _serializer.save<WebcamsConfigsList, cereal::JSONOutputArchive>(_configs);

                        // Trouver la requete correspodnante si elle existe, et si oui détruire sa capture car elle n'est plus valide
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
                // ImGui::SameLine();
                // ImGui::Text("{}\n", infos.name.c_str());
            }
        });
    });
}

} // namespace Cool