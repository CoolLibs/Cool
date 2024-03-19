#include "Variable_TextureDescriptor_FromVideoFile.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_FromVideoFile>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_FromVideoFile>&) -> bool
{
    return false;
}

} // namespace Cool