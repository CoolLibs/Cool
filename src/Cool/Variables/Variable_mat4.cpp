#include "Variable_mat4.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<glm::mat4>& var) -> bool
{
    return internal::imgui_widget(var.name().c_str(), &var.value(), var.metadata().bounds);
}

auto imgui_widget(VariableMetadata<glm::mat4>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool