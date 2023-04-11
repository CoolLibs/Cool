#include "Cool/DidYouKnow/testDidYouKnowModal.hpp"
#include <fmt/chrono.h>
#include <imgui.h>
#include "Cool/DidYouKnow/didYouKnowModal.hpp"

void Cool::test_did_you_know(Cool::DidYouKnow& _did_you_know, std::span<const char* const>& all_tips)
{
    if (ImGui::Button("Test DidYouKnow"))
    {
        _did_you_know.open(all_tips);
    }

    _did_you_know.imgui_windows(all_tips);

    // imgui text with next time of opening
    ImGui::TextUnformatted(fmt::format("Next opening: {:%H:%M:%S}", (_did_you_know._timestamp_last_opening + timeToWait()).time_since_epoch()).c_str());

    // imgui text of current timestamp of did you know
    ImGui::TextUnformatted(fmt::format("Current timestamp: {}", _did_you_know._timestamp_last_opening.time_since_epoch()).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::TextUnformatted(fmt::format("Time to wait: {}", timeToWait()).c_str());
}