#include "Variable_TextureDescriptor_Video.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_Video>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_Video>&) -> bool
{
    return false;
}

} // namespace Cool