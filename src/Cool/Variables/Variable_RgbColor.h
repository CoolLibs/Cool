#pragma once

#include <Cool/StrongTypes/RgbColor.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

namespace Cool {

#include "generated/Variable_RgbColor.inl"

inline auto imgui_widget(Variable<RgbColor>& var) -> bool
{
    return imgui_color_widget(
        var.name,
        var.value,
        rgb_color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<RgbColor>& meta) -> bool
{
    return rgb_color_metadata_widget(meta.is_hdr);
}

} // namespace Cool
