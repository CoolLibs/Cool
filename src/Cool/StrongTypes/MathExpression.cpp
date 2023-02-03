#include "MathExpression.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Cool/StrongTypes/MathExpression.h"

namespace Cool {

auto imgui_widget(std::string_view name, MathExpression& math_expression) -> bool
{
    return ImGui::InputText(name.data(), &math_expression.expression);
}

} // namespace Cool
