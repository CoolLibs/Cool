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
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {12.f, ImGui::GetStyle().FramePadding.y}); // HACK to force the icon to be centered.
        if (ImGui::Button(ICOMOON_PREVIOUS2, button_size, ImDrawFlags_RoundCornersLeft))
        {
            clock.set_time(0.f);
        }
        ImGui::PopStyleVar();
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
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::DragFloat("##time", &t, 0.01f, 0.f, 0.f, "%.2f seconds"))
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
