#include "TextureDescriptor.h"
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/NfdFileFilter/NfdFileFilter.h"

namespace Cool {

auto TextureDescriptor::imgui_widget() -> bool
{
    bool b = false;
    b |= ::Cool::imgui_widget(source);
    b |= sampler.imgui_widget();
    return b;
}

} // namespace Cool