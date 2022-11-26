#pragma once

#include <Cool/StrongTypes/PremultipliedRgbaColor.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

namespace Cool {

#include "generated/Variable_PremultipliedRgbaColor.inl"

inline auto imgui_widget(Variable<PremultipliedRgbaColor>& var) -> bool
{
    return imgui_color_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<PremultipliedRgbaColor>& meta) -> bool
{
    return internal::rgba_color_metadata_widget(meta.is_hdr);
}

} // namespace Cool
