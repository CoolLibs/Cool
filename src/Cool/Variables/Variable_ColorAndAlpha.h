#pragma once

#include <Cool/StrongTypes/ColorAndAlpha.h>
#include "Variable.h"
#include "internal/color_utils.h"

namespace Cool {

#include "generated/Variable_ColorAndAlpha.inl"

inline auto imgui_widget(Variable<ColorAndAlpha>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<ColorAndAlpha>& meta) -> bool
{
    return internal::color_and_alpha_metadata_widget(meta.is_hdr);
}

} // namespace Cool
