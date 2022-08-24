#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/Gradient.h>
#include "Variable.h"
#include "internal/rgb_color_utils.h"

namespace Cool {

#include "generated/Variable_Gradient.inl"

inline auto imgui_widget(Variable<Gradient>& var) -> bool
{
    return gradient_widget(
        var.name,
        var.value,
        var.metadata.should_use_a_random_color_for_the_new_marks,
        rgb_color_imgui_flags(var.metadata.is_hdr)
    );
}

inline auto imgui_widget(VariableMetadata<Gradient>& meta) -> bool
{
    return rgb_color_metadata_widget(meta.is_hdr) |
           ImGG::random_mode_widget("Use random colors when adding new marks", &meta.should_use_a_random_color_for_the_new_marks);
}

} // namespace Cool
