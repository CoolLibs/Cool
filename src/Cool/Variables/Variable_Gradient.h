#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/Gradient.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

namespace Cool {

#include "generated/Variable_Gradient.inl"

inline auto imgui_widget(Variable<Gradient>& var) -> bool
{
    return imgui_widget(
        var.name,
        var.value,
        var.metadata.randomize_new_marks_colors,
        internal::rgb_color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<Gradient>& meta) -> bool
{
    bool b = internal::rgb_color_metadata_widget(meta.is_hdr);
    b |= ImGG::random_mode_widget("Use random colors when adding new marks", &meta.randomize_new_marks_colors);
    return b;
}

} // namespace Cool
