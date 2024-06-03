#include "Variable_Time.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<Time>& var) -> bool
{
    return imgui_drag_time(
        var.name().c_str(), &var.value(),
        {
            .show_milliseconds = var.metadata().show_milliseconds,
            .drag_speed        = var.metadata().drag_speed,
            .min               = var.metadata().has_min_bound ? var.metadata().min : -DBL_MAX,
            .max               = var.metadata().has_max_bound ? var.metadata().max : +DBL_MAX,
        }
    );
}

auto imgui_widget(VariableMetadata<Time>& meta) -> bool
{
    bool b = false;
    b |= internal::imgui_min_max_bounds(meta.min, meta.max, meta.has_min_bound, meta.has_max_bound, meta.drag_speed);
    b |= ImGuiExtras::toggle("Show milliseconds", &meta.show_milliseconds);
    return b;
}

} // namespace Cool