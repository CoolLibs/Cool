#include "Variable_TextureDescriptor_FromFile.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_FromFile>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_FromFile>&) -> bool
{
    return false;
}

} // namespace Cool