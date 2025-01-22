#pragma once
#include "Cool/Exporter/VideoExportOverwriteBehaviour.h"
#include "Cool/ImGui/ColorThemes.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/JsonAutoSerializer.hpp"
#include "Cool/Time/Time.hpp"

namespace Cool {

struct UserSettings {
    auto autosave_enabled() -> bool&;
    auto autosave_delay() -> Time&;
    auto extra_icons() -> bool&;
    auto camera2D_zoom_sensitivity() -> float&;
    auto single_click_to_input_in_drag_widgets() -> bool&;
    auto enable_multi_viewport() -> bool&;
    auto video_export_overwrite_behaviour() -> VideoExportOverwriteBehaviour&;

    Cool::ColorThemes color_themes{};

    void apply_multi_viewport_setting();

    auto imgui() -> bool;
    auto imgui_autosave() -> bool;
    auto imgui_extra_icons() -> bool;
    auto imgui_camera2D_zoom_sensitivity() -> bool;
    auto imgui_single_click_to_input_in_drag_widgets() -> bool;
    auto imgui_enable_multi_viewport() -> bool;
    auto imgui_video_export_overwrite_behaviour() -> bool;

    void update() { _json.update(); }

private:
    JsonAutoSerializer<bool, float, Time, VideoExportOverwriteBehaviour> _json{Cool::Path::user_data() / "user_settings.json"};
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Cool
