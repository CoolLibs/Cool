#include "ImGuiExtrasStyle.h"
#include <imgui.h>
#include "Cool/ImGui/ImGuiExtras.h"

namespace Cool::ImGuiExtras {

void Style::imgui()
{
#include "generated_style/style_imgui.inl"
}

} // namespace Cool::ImGuiExtras