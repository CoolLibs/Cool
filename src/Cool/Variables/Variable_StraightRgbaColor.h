#pragma once

#include <Cool/StrongTypes/StraightRgbaColor.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

namespace Cool {

#include "generated/Variable_StraightRgbaColor.inl"

inline auto imgui_widget(Variable<StraightRgbaColor>& var) -> bool
{
    return imgui_color_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<StraightRgbaColor>& meta) -> bool
{
    return internal::rgba_color_metadata_widget(meta.is_hdr);
}

} // namespace Cool
