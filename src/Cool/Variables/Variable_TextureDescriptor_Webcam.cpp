#include "Variable_TextureDescriptor_Webcam.h"

namespace Cool {

auto imgui_widget(Variable<TextureDescriptor_Webcam>& var) -> bool
{
    return var.value().imgui_widget();
}

auto imgui_widget(VariableMetadata<TextureDescriptor_Webcam>&) -> bool
{
    return false;
}

} // namespace Cool