#pragma once

#include <Cool/StrongTypes/ColorPalette.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

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
    return imgui_widget(
        var.name,
        var.value,
        rgb_color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<ColorPalette>& meta) -> bool
{
    return rgb_color_metadata_widget(meta.is_hdr);
}

} // namespace Cool
