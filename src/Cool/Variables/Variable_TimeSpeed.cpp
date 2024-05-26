#include "Variable_TimeSpeed.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<TimeSpeed>& var) -> bool
{
    // TODO(Time) Don't use the Bound type as metadata, since we don't want the booleans for is_logarithmic and use_slider
    return imgui_widget(var.name().c_str(), &var.value(), var.metadata().bounds.drag_speed, var.metadata().bounds.min, var.metadata().bounds.max);
}

auto imgui_widget(VariableMetadata<TimeSpeed>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool