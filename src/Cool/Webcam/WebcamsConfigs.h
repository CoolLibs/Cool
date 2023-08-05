#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/Path/Path.h>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Serialization/SerializerOnDemand.h"
#include "internal/WebcamConfig.h"
#include "webcam_info/webcam_info.hpp"

namespace Cool {

class WebcamsConfigs {
public:
    [[nodiscard]] static auto instance() -> WebcamsConfigs&;

    auto get_resolution_from_index(size_t index) -> std::optional<webcam_info::Resolution>;
    auto get_resolution_from_name(const std::string& name) -> webcam_info::Resolution;
    void open_webcams_config_window();
    void imgui_windows();
    auto get_config_from_name(const std::string& name) -> WebcamConfig&;

private:
    WebcamsConfigs() = default; // This is a singleton. Get the global instance with `instance()` instead.
    static auto gen_instance() -> WebcamsConfigs&;

private:
    ImGuiWindow              _webcam_config_window{icon_fmt("Webcams Config", ICOMOON_COG)};
    WebcamsConfigsList       _list_webcam_configs{};
    Cool::SerializerOnDemand _serializer{Cool::Path::user_data() / "webcams-configs.json", "Configs"};
};

} // namespace Cool