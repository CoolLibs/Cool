#include "Variable_TextureInfo.h"

namespace Cool {

auto imgui_widget(Variable<TextureInfo>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureInfo>&) -> bool
{
    return false;
}

} // namespace Cool