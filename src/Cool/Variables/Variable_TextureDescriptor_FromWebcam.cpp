#include "Variable_TextureDescriptor_FromWebcam.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_FromWebcam>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_FromWebcam>&) -> bool
{
    return false;
}

} // namespace Cool