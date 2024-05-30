#include "Variable_Time.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<Time>& var) -> bool
{
    // TODO(Time) Add some metadata?
    return ImGuiExtras::drag_time(var.name().c_str(), &var.value());
    // return imgui_widget(var.name().c_str(), &var.value(), var.metadata().bounds.drag_speed, var.metadata().bounds.min, var.metadata().bounds.max);
}

auto imgui_widget(VariableMetadata<Time>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool