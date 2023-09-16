#include "WebcamsConfigs.h"
#include <cereal/archives/json.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>
#include <webcam_info/webcam_info.hpp>
#include "Cool/Image/AspectRatio.h"
#include "TextureLibrary_FromWebcam.h"
#include "WebcamsInfos.h"

namespace Cool {

auto WebcamsConfigs::gen_instance() -> WebcamsConfigs&
{
    static auto inst      = WebcamsConfigs{};
    auto const  maybe_err = inst._serializer.load<WebcamsConfigsMap, cereal::JSONInputArchive>(inst._configs);
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

auto WebcamsConfigs::get_config(std::string const& webcam_name, bool do_lock) -> WebcamConfig&
{
    auto const it = _configs.find(webcam_name);
    if (it != _configs.end())
        return it->second;

    // If none already exists, create a config with the default resolution for that webcam.
    return _configs.emplace(
                       webcam_name,
                       WebcamConfig{WebcamsInfos::instance().default_resolution(webcam_name, do_lock)}
    )
        .first->second;
}

void WebcamsConfigs::open_imgui_window()
{
    _window.open();
}

void WebcamsConfigs::imgui_window()
{
    auto const format_resolution = [](webcam_info::Resolution resolution) {
        return fmt::format(
            "{} x {} ({})",
            resolution.width,
            resolution.height,
            string_from_ratio(static_cast<float>(resolution.width) / static_cast<float>(resolution.height))
        );
    };

    _window.show([&]() {
        WebcamsInfos::instance().for_each_webcam_info([&](webcam_info::Info const& info) {
            auto&      config        = get_config(info.name, /*do_lock=*/false /*for_each_webcam_info already locks*/);
            auto const combo_preview = format_resolution(config.resolution);

            if (ImGui::BeginCombo(info.name.c_str(), combo_preview.c_str()))
            {
                for (auto const& resolution : info.available_resolutions)
                {
                    bool const is_selected = config.resolution == resolution;
                    if (ImGui::Selectable(format_resolution(resolution).c_str(), is_selected))
                    {
                        config.resolution = resolution;
                        TextureLibrary_FromWebcam::instance().invalidate_request(info.name); // Destroy the current request so that a new one will be created with the new requested resolution.
                        _serializer.save<WebcamsConfigsMap, cereal::JSONOutputArchive>(_configs);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
        });
    });
}

} // namespace Cool