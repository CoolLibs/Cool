#include "ClockU.h"
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui.h>
#include "Clock.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "parse_time.h"
#include "time_formatted_hms.h"

namespace Cool::ClockU {

static void parse_time_imgui(const char* buf, void* data_p)
{
    *(static_cast<float*>(data_p)) = parse_time(buf);
}

void imgui_timeline(Cool::Clock& clock, std::function<void()> const& extra_widgets, std::function<void()> const& on_time_reset)
{
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, {0.f, 0.f});
    {
        auto const button_size = ImVec2{1.1f * ImGui::GetFrameHeight(), ImGui::GetFrameHeight()};
        // Reset time
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {12.f, ImGui::GetStyle().FramePadding.y}); // HACK to force the icon to be centered.
        if (ImGui::Button(ICOMOON_PREVIOUS2, button_size, ImDrawFlags_RoundCornersLeft))
        {
            clock.set_time(0.f, /*force_delta_time_to_ignore_the_change=*/true);
            on_time_reset();
        }
        ImGui::PopStyleVar();
        ImGui::SetItemTooltip("%s", "Reset time to 0.");

        ImGuiExtras::join_buttons();

        // Play / Pause
        if (ImGui::Button(clock.is_playing() ? ICOMOON_PAUSE2 : ICOMOON_PLAY3, button_size, ImDrawFlags_RoundCornersRight))
        {
            clock.toggle_play_pause();
        }
    }
    ImGui::PopStyleVar();

    ImGui::SameLine();
    extra_widgets();
    ImGui::SameLine();

    // Timeline
    float t = clock.time_in_seconds();
    ImGui::PushFont(Font::monospace());
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::DragFloat("##time", &t, 0.01f, 0.f, 0.f, time_formatted_hms(t, true /*show_milliseconds*/).c_str(), 0, &parse_time_imgui))
        clock.set_time(t);
    clock.set_is_being_changed_in_GUI(ImGui::IsItemActive());
    ImGui::PopFont();
}

void imgui_display_time(Cool::Clock& clock)
{
    ImGui::Text("%.2f seconds", clock.time_in_seconds());
}

} // namespace Cool::ClockU
