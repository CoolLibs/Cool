#include "Variable_MathExpression.h"
#include "Cool/StrongTypes/MathExpression.h"
#include "Cool/Variables/internal/BoundsMetadataWidget.h"

namespace Cool {

auto imgui_widget(Variable<Cool::MathExpression>& var) -> bool
{
    return imgui_widget(var.name.c_str(), var.value);
}
auto imgui_widget(VariableMetadata<Cool::MathExpression>&) -> bool
{
    return false;
}

} // namespace Cool