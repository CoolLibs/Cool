#include "Hue.h"
#include "imgui.h"

namespace Cool {

auto Hue::imgui_widget(std::string_view name) -> bool
{
    bool b = false;
    ImGui::PushID(name.data());

    b |= ImGuiExtras::hue_wheel(name.data(), &value);

    ImGui::SameLine();

    b |= ImGui::SliderFloat("", &value, 0.f, 1.f);

    ImGui::PopID();
    return b;
}

} // namespace Cool