#include "Variable_WebcamInfo.h"

namespace Cool {

auto imgui_widget(Variable<WebcamInfo>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<WebcamInfo>&) -> bool
{
    return false;
}

} // namespace Cool