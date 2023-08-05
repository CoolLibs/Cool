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

    [[nodiscard]] auto selected_resolution(std::string const& webcam_name) -> webcam_info::Resolution;

    void open_imgui_window();
    void imgui_window();

private:
    WebcamsConfigs() = default; // This is a singleton. Get the global instance with `instance()` instead.
    static auto gen_instance() -> WebcamsConfigs&;

    [[nodiscard]] auto get_config(std::string const& webcam_name, bool do_lock = true) -> WebcamConfig&;

private:
    ImGuiWindow              _window{icon_fmt("Webcams Configs", ICOMOON_COG)};
    WebcamsConfigsList       _configs{};
    Cool::SerializerOnDemand _serializer{Cool::Path::user_data() / "webcams-configs.json", "Configs"};
};

} // namespace Cool