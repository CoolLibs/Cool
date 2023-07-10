#pragma once
#include "Cool/ImGui/ColorThemes.h"

namespace Cool {

struct UserSettings {
    bool  autosave_enabled{true};
    float autosave_delay_in_seconds{5.f};

    bool extra_icons{true};

    float camera2D_zoom_sensitivity{1.1f};

    Cool::ColorThemes color_themes{};

    bool single_click_to_input_in_drag_widgets{false};

    bool enable_multi_viewport
    {
#if defined(__linux__)
        false // On Linux this can conflict with tiling WM and make our context menus behave weirdly.
              // https://cdn.discordapp.com/attachments/848704719987671070/1127711921651597332/ui-linux.mp4
              // https://github.com/ocornut/imgui/issues/2117
#else
        true
#endif
    };
    void apply_multi_viewport_setting() const;

    auto imgui() -> bool;
    auto imgui_autosave() -> bool;
    auto imgui_extra_icons() -> bool;
    auto imgui_camera2D_zoom_sensitivity() -> bool;
    auto imgui_single_click_to_input_in_drag_widgets() -> bool;
    auto imgui_enable_multi_viewport() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Autosave enabled", autosave_enabled),
            cereal::make_nvp("Autosave delay in seconds", autosave_delay_in_seconds),
            cereal::make_nvp("Extra icons", extra_icons),
            cereal::make_nvp("Camera 2D zoom sensitivity", camera2D_zoom_sensitivity),
            cereal::make_nvp("Use OS color theme", color_themes),
            cereal::make_nvp("Single click to input in drag widgets", single_click_to_input_in_drag_widgets),
            cereal::make_nvp("Enable multi-viewport", enable_multi_viewport)
        );
    }
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Cool
