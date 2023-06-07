#include "Cool/DidYouKnow/testDidYouKnow.h"
#include <fmt/chrono.h>
#include <imgui.h>
#include "Cool/DidYouKnow/DidYouKnow.h"
#include "time_to_wait.h"

void Cool::test_did_you_know(Cool::DidYouKnow& _did_you_know, std::span<const char* const> all_tips)
{
    if (ImGui::Button("Open a \"Did you know?\""))
    {
        _did_you_know.open_popup(all_tips);
    }

    _did_you_know.imgui_windows(all_tips);

    // imgui text with next time of opening
    auto next_opening = (_did_you_know._timestamp_last_opening + time_to_wait()).time_since_epoch();
    ImGui::TextUnformatted(fmt::format("Next opening: {:%OH}h {:%OM}min {:%OS}s", next_opening, next_opening, next_opening).c_str());

    // imgui text of current timestamp of did you know
    ImGui::TextUnformatted(fmt::format("Current timestamp: {:%OH:%OM:%OS}", _did_you_know._timestamp_last_opening.time_since_epoch()).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::TextUnformatted(fmt::format("Time to wait: {}", time_to_wait()).c_str());

    auto time_remaining = time_to_wait() - (std::chrono::steady_clock::now() - _did_you_know._timestamp_last_opening);
    ImGui::TextUnformatted(fmt::format("Time remaining: {:%OH}h {:%OM}min {:%OS}s", time_remaining, time_remaining, time_remaining).c_str());
}