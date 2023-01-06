#include "Variable_vec3.h"
#include "Cool/Variables/internal/float_utils.h"

namespace Cool {

auto imgui_widget(Variable<glm::vec3>& var) -> bool
{
    if (var.metadata.bounded)
    {
        return ImGui::SliderFloat3(
            var.name.c_str(),
            glm::value_ptr(var.value),
            var.metadata.min_value,
            var.metadata.max_value
        );
    }
    else // NOLINT(readability-else-after-return)
    {
        return ImGui::DragFloat3(
            var.name.c_str(),
            glm::value_ptr(var.value),
            var.metadata.drag_speed
        );
    }
}

auto imgui_widget(VariableMetadata<glm::vec3>& meta) -> bool
{
    return internal::float_metadata_widget(meta.min_value, meta.max_value, meta.bounded, meta.drag_speed);
}

} // namespace Cool