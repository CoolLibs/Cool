#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/Path/Path.h>
#include <wcam/src/KeepLibraryAlive.hpp>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Serialization/SerializerOnDemand.h"
#include "internal/WebcamConfig.h"
#include "wcam/wcam.hpp"

namespace Cool {

// using WebcamsConfigsMap = std::unordered_map<std::string, WebcamConfig>;

class WebcamsConfigs {
public:
    [[nodiscard]] static auto instance() -> WebcamsConfigs&;

    // [[nodiscard]] auto selected_resolution(std::string const& webcam_name) -> wcam::Resolution;

    void open_imgui_window();
    void imgui_window();

private:
    WebcamsConfigs() = default; // This is a singleton. Get the global instance with `instance()` instead.
    static auto gen_instance() -> WebcamsConfigs&;

    // [[nodiscard]] auto get_config(std::string const& webcam_name, bool do_lock = true) -> WebcamConfig&;

private:
    // WebcamsConfigsMap        _configs{};
    wcam::KeepLibraryAlive   _keep_wcam_lib_alive{};
    ImGuiWindow              _window{icon_fmt("Webcams", ICOMOON_VIDEO_CAMERA)};
    Cool::SerializerOnDemand _serializer{Cool::Path::user_data() / "webcams-configs.json", "Configs"};

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Window", _window)
        );
    }
};

} // namespace Cool