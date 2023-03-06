#include "Variable_Gradient.h"
#include <Cool/ImGui/ImGuiExtras.h>
#include "internal/color_utils.h"

namespace Cool {

auto imgui_widget(Variable<Gradient>& var) -> bool
{
    return imgui_widget(
        var.name(),
        var.value(),
        var.metadata().randomize_new_marks_colors,
        internal::color_imgui_flags(var.metadata().is_hdr)
    );
}

auto imgui_widget(VariableMetadata<Gradient>& meta) -> bool
{
    bool b = internal::color_metadata_widget(meta.is_hdr);
    b |= ImGG::random_mode_widget("Use random colors when adding new marks", &meta.randomize_new_marks_colors);
    return b;
}

} // namespace Cool