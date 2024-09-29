#pragma once
#include <Cool/ImGui/IcoMoonCodepoints.h>
#include <Cool/Path/Path.h>
#include "Cool/ImGui/ImGuiWindow.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Serialization/SerializerOnDemand.h"
#include "wcam/wcam.hpp"

namespace Cool {

class WebcamsConfigs {
public:
    [[nodiscard]] static auto instance() -> WebcamsConfigs&;

    void open_imgui_window();
    void imgui_window();

private:
    WebcamsConfigs() = default; // This is a singleton. Get the global instance with `instance()` instead.
    static auto gen_instance() -> WebcamsConfigs&;

private:
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