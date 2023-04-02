#include "didYouKnowModal.hpp"
#include <imgui.h>
#include <string>

void DidYouKnowModal::open()
{
    auto difference = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - std::chrono::system_clock::to_time_t(_current_timestamp);

    int time_to_wait = 1 * 60 * 60; // 1 hour

    if (!_has_been_opened && difference > time_to_wait)
    {
        _has_been_opened = true;
        ImGui::OpenPopup(_id.c_str());
        _current_timestamp = std::chrono::system_clock::now();
        return;
    }
}

void DidYouKnowModal::prepare_next_tip()
{
    _current_tip_index++;
    if (_current_tip_index >= _all_tips.size())
        _current_tip_index = 0;
    _text = _all_tips[_current_tip_index];
}

auto DidYouKnowModal::is_open() const -> bool
{
    return ImGui::BeginPopupModal(_id.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);
}

void DidYouKnowModal::imgui_window()
{
    ImGui::Text("%s", _text.c_str());
    ImGui::Separator();

    if (ImGui::Button("OK", ImVec2(120, 0)))
    {
        ImGui::CloseCurrentPopup();
        prepare_next_tip();
    }

    ImGui::SameLine();
    ImGui::SetItemDefaultFocus();

    if (ImGui::Button("Show all tips", ImVec2(120, 0)))
    {
        ImGui::OpenPopup("All tips");
    }

    all_tips();
    ImGui::EndPopup();
}

void DidYouKnowModal::all_tips()
{
    if (ImGui::BeginPopupModal("All tips", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        for (const auto& tip : _all_tips)
        {
            ImGui::Text("%s", tip.c_str());
            ImGui::Separator();
        }
        if (ImGui::Button("Got it !"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void test_did_you_know(DidYouKnowModal& _did_you_know_)
{
    _did_you_know_.open();

    if (_did_you_know_.is_open())
    {
        _did_you_know_.imgui_window();
    }
}

void debug_did_you_know(DidYouKnowModal& _did_you_know_)
{
    if (ImGui::Button("Test DidYouKnow"))
    {
        _did_you_know_._has_been_opened   = false;
        _did_you_know_._current_timestamp = std::chrono::system_clock::now() - std::chrono::hours(2);
    }

    // imgui text with difference between current timestamp of did you know and current timestamp of now
    ImGui::Text("Difference: %s", std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - std::chrono::system_clock::to_time_t(_did_you_know_._current_timestamp)).c_str());

    // imgui text of current timestamp of did you know
    ImGui::Text("Current timestamp: %s", std::to_string(std::chrono::system_clock::to_time_t(_did_you_know_._current_timestamp)).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::Text("Time to wait: %s", std::to_string(1 * 60 * 60).c_str());
}