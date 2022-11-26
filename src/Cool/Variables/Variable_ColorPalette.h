#pragma once

#include <Cool/StrongTypes/ColorPalette.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

namespace Cool {

#include "generated/Variable_ColorPalette.inl"

inline auto imgui_widget(Variable<ColorPalette>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<ColorPalette>& meta) -> bool
{
    return internal::rgb_color_metadata_widget(meta.is_hdr);
}

} // namespace Cool
