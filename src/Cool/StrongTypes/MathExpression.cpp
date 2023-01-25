#include "MathExpression.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "Cool/StrongTypes/MathExpression.h"

namespace Cool {

auto imgui_widget(std::string_view name, MathExpression& math_expression) -> bool
{
    return ImGui::InputText(name.data(), &math_expression.expression, ImGuiInputTextFlags_EnterReturnsTrue)
           || ImGui::IsItemActivated();
    // Required for the history to work properly. Since we only return true when enter is pressed, it is possible that the history misses some changes to the variable. This is why we force the history to save the state of the variable when someone is about to type into the text input.
}

} // namespace Cool
