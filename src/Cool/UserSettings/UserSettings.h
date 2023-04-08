#pragma once
#include <imgui-node-editor/imgui_node_editor.h>
#include "Cool/ImGui/ColorThemes.h"
#include "Cool/Nodes/blueprints-example.h"

namespace Cool {

struct UserSettings {
    bool  autosave_enabled{true};
    float autosave_delay_in_seconds{5.f};
    float camera2D_zoom_sensitivity{1.1f};

    Cool::ColorThemes color_themes{};

    auto imgui() -> bool;

    auto imgui_autosave() -> bool;
    auto imgui_camera2D_zoom_sensitivity() -> bool;

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
            cereal::make_nvp("Use OS color theme", color_themes),
            cereal::make_nvp("ImNodes style", ax::NodeEditor::GetStyle()) // TODO(JF) Serialize this elsewhere, not in the UserSettings
        );
    }
};

inline auto user_settings() -> UserSettings&
{
    static UserSettings instance;
    return instance;
}

} // namespace Cool
