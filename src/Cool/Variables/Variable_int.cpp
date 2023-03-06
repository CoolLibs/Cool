#include "Variable_int.h"
#include "imgui.h"
#include "internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<int>& var) -> bool
{
    return internal::imgui_widget(var.name().c_str(), &var.value(), var.metadata().bounds);
}

auto imgui_widget(VariableMetadata<int>& meta) -> bool
{
    return internal::imgui_widget(meta.bounds);
}

} // namespace Cool