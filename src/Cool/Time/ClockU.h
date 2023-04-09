#pragma once
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui.h>
#include "Clock.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"

namespace Cool::ClockU {

void imgui_timeline(auto& clock)
{
    if (ImGui::Button(clock.is_playing() ? ICOMOON_PAUSE2 : ICOMOON_PLAY3))
    {
        clock.toggle_play_pause();
    }

    ImGui::SameLine();
    if (ImGui::Button(ICOMOON_PREVIOUS2))
    {
        clock.set_time(0.f);
    }
    Cool::ImGuiExtras::tooltip("Reset time to 0.");

    ImGui::SameLine();
    float t = clock.time();
    ImGui::PushFont(Font::monospace());
    if (ImGui::DragFloat("##time", &t, 0.5f, 0.f, 0.f, "%.2f seconds"))
    {
        clock.set_time(t);
    }
    ImGui::PopFont();
}

void imgui_display_time(auto& clock)
{
    ImGui::Text("%.2f seconds", clock.time());
}

} // namespace Cool::ClockU
