#pragma once
#include "Cool/Exporter/VideoExportOverwriteBehaviour.h"
#include "Cool/ImGui/ColorThemes.h"
#include "Cool/Time/Time.hpp"

namespace Cool {

auto should_enable_multi_viewport_by_default() -> bool;

struct UserSettings {
    bool autosave_enabled{true};
    Time autosave_delay{5s};

    bool extra_icons{true};

    float camera2D_zoom_sensitivity{1.1f};

    Cool::ColorThemes color_themes{};

    bool single_click_to_input_in_drag_widgets{true};

    bool enable_multi_viewport{should_enable_multi_viewport_by_default()};
    void apply_multi_viewport_setting() const;

    VideoExportOverwriteBehaviour video_export_overwrite_behaviour{VideoExportOverwriteBehaviour::AskBeforeCreatingNewFolder};

    auto imgui() -> bool;
    auto imgui_autosave() -> bool;
    auto imgui_extra_icons() -> bool;
    auto imgui_camera2D_zoom_sensitivity() -> bool;
    auto imgui_single_click_to_input_in_drag_widgets() -> bool;
    auto imgui_enable_multi_viewport() -> bool;
    auto imgui_video_export_overwrite_behaviour() -> bool;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Autosave enabled", autosave_enabled),
            ser20::make_nvp("Autosave delay", autosave_delay),
            ser20::make_nvp("Extra icons", extra_icons),
            ser20::make_nvp("Camera 2D zoom sensitivity", camera2D_zoom_sensitivity),
            ser20::make_nvp("Single click to input in drag widgets", single_click_to_input_in_drag_widgets),
            ser20::make_nvp("Enable multi-viewport", enable_multi_viewport),
            ser20::make_nvp("Video export overwrite behaviour", video_export_overwrite_behaviour)
        );
    }
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Cool
