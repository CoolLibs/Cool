#include "Variable_vec4.h"

namespace Cool {

auto imgui_widget(Variable<glm::vec4>& var) -> bool
{
    return ImGui::DragFloat4(
        var.name.c_str(),
        glm::value_ptr(var.value),
        var.metadata.drag_speed
    );
}

auto imgui_widget(VariableMetadata<glm::vec4>& meta) -> bool
{
    return ImGui::DragFloat("Drag speed", &meta.drag_speed, 0.01f, 0.001f, FLT_MAX / static_cast<float>(INT_MAX));
}

} // namespace Cool