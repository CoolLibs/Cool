#include "Variable_TextureDescriptor_ImageSequence.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_ImageSequence>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_ImageSequence>&) -> bool
{
    return false;
}

} // namespace Cool