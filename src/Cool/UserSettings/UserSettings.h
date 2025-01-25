#pragma once
#include <smart/smart.hpp>
#include "Cool/Exporter/VideoExportOverwriteBehaviour.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Json.hpp"
#include "Cool/Serialization/JsonAutoSerializer.hpp"
#include "Cool/Time/Time.hpp"

namespace Cool {

auto should_enable_multi_viewport_by_default() -> bool;

struct UserSettings {
    bool                          autosave_enabled{true};
    Time                          autosave_delay{5s};
    bool                          extra_icons{true};
    float                         camera2D_zoom_sensitivity{1.1f};
    bool                          single_click_to_input_in_drag_widgets{true};
    bool                          enable_multi_viewport{should_enable_multi_viewport_by_default()};
    VideoExportOverwriteBehaviour video_export_overwrite_behaviour{VideoExportOverwriteBehaviour::AskBeforeCreatingNewFolder};

    void apply_multi_viewport_setting();

    auto imgui() -> bool;
    auto imgui_autosave() -> bool;
    auto imgui_extra_icons() -> bool;
    auto imgui_camera2D_zoom_sensitivity() -> bool;
    auto imgui_single_click_to_input_in_drag_widgets() -> bool;
    auto imgui_enable_multi_viewport() -> bool;
    auto imgui_video_export_overwrite_behaviour() -> bool;

    void update() { _serializer.update(); }

private:
    // Must be declared last, after all the variables it serializes, so that the values it loads overwrite the default values, and not the other way around
    JsonAutoSerializer _serializer{
        Cool::Path::user_data() / "user_settings.json",
        [&](nlohmann::json const& json) {
            json_get(json, "Autosave enabled", autosave_enabled);
            json_get(json, "Autosave delay", autosave_delay);
            autosave_delay = Time::seconds(smart::keep_above(1., autosave_delay.as_seconds_double())); // Make sure the delay is at least 1 second, to avoid lagging
            json_get(json, "Extra icons", extra_icons);
            json_get(json, "Camera 2D zoom sensitivity", camera2D_zoom_sensitivity);
            json_get(json, "Single click to input in drag widgets", single_click_to_input_in_drag_widgets);
            json_get(json, "Enable multi viewport", enable_multi_viewport);
            json_get(json, "Video export overwrite behaviour", video_export_overwrite_behaviour);
        },
        [&](nlohmann::json& json) {
            json_set(json, "Autosave enabled", autosave_enabled);
            json_set(json, "Autosave delay", autosave_delay);
            json_set(json, "Extra icons", extra_icons);
            json_set(json, "Camera 2D zoom sensitivity", camera2D_zoom_sensitivity);
            json_set(json, "Single click to input in drag widgets", single_click_to_input_in_drag_widgets);
            json_set(json, "Enable multi viewport", enable_multi_viewport);
            json_set(json, "Video export overwrite behaviour", video_export_overwrite_behaviour);
        }
    };
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Cool
