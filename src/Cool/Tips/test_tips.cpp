#include "Cool/Tips/test_tips.h"
#include <fmt/chrono.h>
#include <imgui.h>
#include "Cool/Tips/TipsManager.h"
#include "time_to_wait.h"

void Cool::test_tips(Cool::TipsManager& tips_manager)
{
    if (ImGui::Button("Open a tip"))
        tips_manager.open_one_tip_window();

    // imgui text with next time of opening
    auto next_opening = (tips_manager._time_when_app_was_last_closed + time_to_wait()).time_since_epoch();
    ImGui::TextUnformatted(fmt::format("Next opening: {:%OH}h {:%OM}min {:%OS}s", next_opening, next_opening, next_opening).c_str());

    // imgui text of current timestamp of did you know
    ImGui::TextUnformatted(fmt::format("Current timestamp: {:%OH:%OM:%OS}", tips_manager._time_when_app_was_last_closed.time_since_epoch()).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::TextUnformatted(fmt::format("Time to wait: {}", time_to_wait()).c_str());

    auto time_remaining = time_to_wait() - (std::chrono::steady_clock::now() - tips_manager._time_when_app_was_last_closed);
    ImGui::TextUnformatted(fmt::format("Time remaining: {:%OH}h {:%OM}min {:%OS}s", time_remaining, time_remaining, time_remaining).c_str());
}