#include "NodesCategoryConfig.h"
#include <filesystem>
#include "Cool/Path/Path.h"
#include "Cool/Serialization/as_json.h"
#include "Cool/Variables/internal/color_utils.h"
#include "imgui.h"

namespace Cool {

void NodesCategoryConfig::save_to_json() const
{
    Serialization::to_json(*this, _path_to_json);
}

void NodesCategoryConfig::load_from_json()
{
    Serialization::from_json(*this, _path_to_json)
        .send_error_if_any([&](std::string const& message) {
            return Message{
                .category = "Nodes Category",
                .message  = fmt::format("Failed to load category config for '{}':\n{}", _path_to_json, message),
                .severity = MessageSeverity::Warning,
            };
        },
                           Log::ToUser::console());
}

auto NodesCategoryConfig::imgui_popup() -> bool
{
    bool b = false;

    if (ImGui::BeginPopupContextItem())
    {
        if (imgui_widget("Color", _color, internal::color_imgui_flags(false)))
        {
            b = true;
            save_to_json();
        }
        ImGui::EndPopup();
    }

    return b;
}

} // namespace Cool