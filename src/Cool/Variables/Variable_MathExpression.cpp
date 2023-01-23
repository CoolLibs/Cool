#include "Variable_MathExpression.h"
#include "Cool/StrongTypes/MathExpression.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<Cool::MathExpression>& var) -> bool
{
    return internal::imgui_widget(var.name.c_str(), &var.value, var.metadata.bounds);
};
auto imgui_widget(VariableMetadata<Cool::MathExpression>& meta) -> bool
{
    return internal::imgui_widget(meta);
}

} // namespace Cool