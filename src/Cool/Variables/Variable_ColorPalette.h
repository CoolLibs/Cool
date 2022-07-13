#pragma once

#include <Cool/StrongTypes/ColorPalette.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<ColorPalette> {
    bool is_hdr = true;

    friend auto operator<=>(const VariableMetadata<ColorPalette>&, const VariableMetadata<ColorPalette>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Is HDR", is_hdr));
    }
};

inline auto imgui_widget(Variable<ColorPalette>& var) -> bool
{
    return imgui_color_palette_widget(
        var.name,
        var.value,
        (var.metadata.is_hdr ? ImGuiColorEditFlags_HDR : 0)
    );
}

inline auto imgui_widget(VariableMetadata<ColorPalette>& meta) -> bool
{
    bool b = false;
    b |= ImGui::Checkbox("HDR", &meta.is_hdr);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Allows the RGB values to go outside of the [0, 1] range.");
    return b;
}

} // namespace Cool
