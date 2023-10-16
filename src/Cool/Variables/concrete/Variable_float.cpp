#include "Variable_float.h"
#include "internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<float>& var) -> bool
{
    return internal::imgui_widget(
        var.name.c_str(),
        &var.value,
        var.metadata.bounds
    );
}

auto imgui_widget(Metadata<float>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool