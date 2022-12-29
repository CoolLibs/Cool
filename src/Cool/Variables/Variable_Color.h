#pragma once

#include <Cool/StrongTypes/Color.h>
#include "Variable.h"
#include "internal/color_utils.h"

namespace Cool {

#include "generated/Variable_Color.inl"

inline auto imgui_widget(Variable<Color>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        internal::color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<Color>& meta) -> bool
{
    return internal::color_metadata_widget(meta.is_hdr);
}

} // namespace Cool
