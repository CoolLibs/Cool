#pragma once
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include "Clock.h"

namespace Cool::ClockU {

void imgui_timeline(auto& clock)
{
    if (ImGuiExtras::button_with_icon((clock.is_playing() ? Icons::pause() : Icons::play()).imgui_texture_id()))
    {
        clock.toggle_play_pause();
    }

    if (ImGui::IsKeyReleased(ImGuiKey_Space) && !ImGui::GetIO().WantTextInput)
    {
        clock.toggle_play_pause();
    }

    ImGui::SameLine();
    float t = clock.time();
    if (ImGui::DragFloat("seconds", &t, 0.5f, 0.f, 0.f, "%.2f"))
    {
        clock.set_time(t);
    }
}

void imgui_display_time(auto& clock)
{
    ImGui::Text("%.2f seconds", clock.time());
}

} // namespace Cool::ClockU
