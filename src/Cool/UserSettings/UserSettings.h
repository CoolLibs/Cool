#pragma once

namespace Cool {

struct UserSettings {
    bool  autosave_enabled{true};
    float autosave_delay_in_seconds{5.f};
    float camera2D_zoom_sensitivity{1.1f};
    bool  enable_multiviewport{true};
    bool  hack_reset_imgui_context_each_frame{false};

    auto imgui() -> bool;

    auto imgui_autosave() -> bool;
    auto imgui_camera2D_zoom_sensitivity() -> bool;
    auto imgui_enable_multiviewport() -> bool;
    auto imgui_hack_reset_imgui_context_each_frame() -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Autosave enabled", autosave_enabled),
            cereal::make_nvp("Autosave delay in seconds", autosave_delay_in_seconds),
            cereal::make_nvp("Camera 2D zoom sensitivity", camera2D_zoom_sensitivity),
            cereal::make_nvp("Enable multi-windows", enable_multiviewport),
            cereal::make_nvp("HACK to fix UI visual glitches", hack_reset_imgui_context_each_frame)
        );
    }
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Cool
