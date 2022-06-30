#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/RgbColor.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<RgbColor> {
    bool is_hdr = true;

    friend auto operator<=>(const VariableMetadata<RgbColor>&, const VariableMetadata<RgbColor>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Is HDR", is_hdr));
    }
};

inline auto imgui_widget(Variable<RgbColor>& var) -> bool
{
    return color_widget(
        var.name,
        var.value.value,
        // Flags:
        ImGuiColorEditFlags_Float |
            (var.metadata.is_hdr ? ImGuiColorEditFlags_HDR : 0)
    );
}

inline auto imgui_widget(VariableMetadata<RgbColor>& meta) -> bool
{
    bool b = false;
    b |= ImGui::Checkbox("HDR", &meta.is_hdr);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows the RGB values to go outside of the [0, 1] range.");
    return b;
}

} // namespace Cool
