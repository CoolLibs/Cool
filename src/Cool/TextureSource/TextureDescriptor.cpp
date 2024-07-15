#include "TextureDescriptor.h"

namespace Cool {

auto TextureDescriptor::imgui_widget() -> bool
{
    bool b = false;
    b |= ::Cool::imgui_widget(source);
    ImGui::NewLine();
    ImGui::SeparatorText("Image options");
    b |= sampler.imgui_widget();
    return b;
}

} // namespace Cool