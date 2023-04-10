#include "ClockU.h"
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui.h>
#include "Clock.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"

namespace Cool::ClockU {

void imgui_timeline(Cool::Clock& clock)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.f, 0.f});
    {
        auto const button_size = ImVec2{1.1f * ImGui::GetFrameHeight(), ImGui::GetFrameHeight()};
        // Reset time
        if (ImGui::Button(ICOMOON_PREVIOUS2, button_size, ImDrawFlags_RoundCornersLeft))
        {
            clock.set_time(0.f);
        }
        Cool::ImGuiExtras::tooltip("Reset time to 0.");

        ImGuiExtras::join_buttons();

        // Play / Pause
        if (ImGui::Button(clock.is_playing() ? ICOMOON_PAUSE2 : ICOMOON_PLAY3, button_size, ImDrawFlags_RoundCornersRight))
        {
            clock.toggle_play_pause();
        }
    }
    ImGui::PopStyleVar();

    ImGui::SameLine();

    // Timeline
    float t = clock.time();
    ImGui::PushFont(Font::monospace());
    if (ImGui::DragFloat("##time", &t, 0.5f, 0.f, 0.f, "%.2f seconds"))
    {
        clock.set_time(t);
    }
    ImGui::PopFont();
}

void imgui_display_time(Cool::Clock& clock)
{
    ImGui::Text("%.2f seconds", clock.time());
}

} // namespace Cool::ClockU
