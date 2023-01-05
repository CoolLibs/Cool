#include "Variable_float.h"
#include "Cool/Variables/internal/float_utils.h"

namespace Cool {

auto imgui_widget(Variable<float>& var) -> bool
{
    if (var.metadata.bounded)
    {
        return ImGui::SliderFloat(
            var.name.c_str(),
            &var.value,
            var.metadata.min_value,
            var.metadata.max_value
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return ImGui::DragFloat(
            var.name.c_str(),
            &var.value,
            var.metadata.drag_speed
        );
    }
}

auto imgui_widget(VariableMetadata<float>& meta) -> bool
{
    return internal::float_metadata_widget(meta.min_value, meta.max_value, meta.bounded, meta.drag_speed);
}

} // namespace Cool