#include "Variable_int.h"
#include "imgui.h"
#include "internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<int>& var) -> bool
{
    if (var.metadata.bounds.is_bounded)
    {
        return ImGui::SliderInt(
            var.name.c_str(),
            &var.value,
            var.metadata.bounds.min,
            var.metadata.bounds.max
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return ImGui::DragInt(
            var.name.c_str(),
            &var.value,
            var.metadata.bounds.drag_speed
        );
    }
}

auto imgui_widget(VariableMetadata<int>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool