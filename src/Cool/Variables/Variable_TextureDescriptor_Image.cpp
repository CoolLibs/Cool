#include "Variable_TextureDescriptor_Image.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_Image>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_Image>&) -> bool
{
    return false;
}

} // namespace Cool