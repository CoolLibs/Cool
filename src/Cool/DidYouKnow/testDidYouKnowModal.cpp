#include "Cool/DidYouKnow/testDidYouKnowModal.hpp"

void test_did_you_know(DidYouKnowModal& _did_you_know)
{
    _did_you_know.open_ifn();

    _did_you_know.imgui_window();
}

void debug_did_you_know(DidYouKnowModal& _did_you_know)
{
    if (ImGui::Button("Test DidYouKnow"))
    {
        _did_you_know.open();
    }

    _did_you_know.imgui_window();

    // imgui text with difference between current timestamp of did you know and current timestamp of now
    ImGui::Text("%s", fmt::format("Difference: {}", (std::chrono::steady_clock::now() - _did_you_know._timestamp_last_opening).count()).c_str());

    // imgui text of current timestamp of did you know
    ImGui::Text("%s", fmt::format("Current timestamp: {} s", _did_you_know._timestamp_last_opening.time_since_epoch().count()).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::Text("Time to wait: %s h", std::to_string(timeToWait.count()).c_str());
}