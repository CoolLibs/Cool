#include "Variable_TextureDescriptor_VideoFile.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_VideoFile>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_VideoFile>&) -> bool
{
    return false;
}

} // namespace Cool