#include "NodesCategoryConfig.h"
#include <filesystem>
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "imgui.h"

void NodesCategoryConfig::save_to_json() const
{
    Cool::Serialization::to_json(
        *this,
        _path_to_json
    );
}

void NodesCategoryConfig::load_from_json()
{
    Cool::Serialization::from_json(*this, _path_to_json)
        .send_error_if_any([&](std::string const& message) {
            return Cool::Message{
                .category = "Nodes Category",
                .message  = fmt::format("Failed to load category config for '{}':\n{}", _path_to_json, message),
                .severity = Cool::MessageSeverity::Warning,
            };
        },
                           Cool::Log::ToUser::console());
}

auto NodesCategoryConfig::imgui_popup() -> bool
{
    bool color_modified = false;

    if (ImGui::BeginPopupContextItem())
    {
        if (imgui_widget("Color :", _color, ImGuiColorEditFlags_PickerHueBar))
        {
            color_modified = true;
            save_to_json();
        }
        ImGui::EndPopup();
    }

    return color_modified;
}