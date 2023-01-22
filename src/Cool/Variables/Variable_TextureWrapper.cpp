#include "Variable_TextureWrapper.h"

namespace Cool {

auto imgui_widget(Variable<TextureWrapper>& var) -> bool
{
    return var.value.imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureWrapper>&) -> bool
{
    return false;
}

} // namespace Cool