#include "Variable_TimeSpeed.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<TimeSpeed>& var) -> bool
{
    return imgui_widget(
        var.name().c_str(), &var.value(), var.metadata().drag_speed,
        var.metadata().has_min_bound ? var.metadata().min : -DBL_MAX,
        var.metadata().has_max_bound ? var.metadata().max : +DBL_MAX
    );
}

auto imgui_widget(VariableMetadata<TimeSpeed>& meta) -> bool
{
    return internal::imgui_min_max_bounds(meta.min, meta.max, meta.has_min_bound, meta.has_max_bound, meta.drag_speed);
}

} // namespace Cool