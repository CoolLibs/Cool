#include "Variable_TimeSpeed.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<TimeSpeed>& var) -> bool
{
    ImGui::PushID(&var);
    ImGui::SetNextItemWidth(70.f);
    auto const res = internal::imgui_widget("", &var.value().value, var.metadata().bounds, "x%.2f");
    ImGui::PopID();
    return res;
}

auto imgui_widget(VariableMetadata<TimeSpeed>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool