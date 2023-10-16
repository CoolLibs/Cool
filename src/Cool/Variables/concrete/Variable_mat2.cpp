#include "Variable_mat2.h"
#include "internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<glm::mat2>& var) -> bool
{
    return internal::imgui_widget(
        var.name.c_str(),
        &var.value,
        var.metadata.bounds
    );
}

auto imgui_widget(Metadata<glm::mat2>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool