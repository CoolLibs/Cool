#include "Cool/DidYouKnow/testDidYouKnowModal.hpp"
#include <fmt/chrono.h>
#include <imgui.h>

void Cool::did_you_know(Cool::DidYouKnowModal& _did_you_know)
{
    _did_you_know.open_ifn();

    _did_you_know.imgui_windows();
}

void Cool::test_did_you_know(Cool::DidYouKnowModal& _did_you_know)
{
    if (ImGui::Button("Test DidYouKnow"))
    {
        _did_you_know.open();
    }

    _did_you_know.imgui_windows();

    // imgui text with difference between current timestamp of did you know and current timestamp of now
    ImGui::TextUnformatted(fmt::format("Difference: {}", (std::chrono::steady_clock::now() - _did_you_know._timestamp_last_opening)).c_str());

    // imgui text of current timestamp of did you know
    ImGui::TextUnformatted(fmt::format("Current timestamp: {}", _did_you_know._timestamp_last_opening.time_since_epoch()).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::TextUnformatted(fmt::format("Time to wait: {}", timeToWait).c_str());
}