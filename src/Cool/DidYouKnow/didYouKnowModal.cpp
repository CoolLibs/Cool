#include "didYouKnowModal.hpp"
#include <imgui.h>
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include "fmt/compile.h"

static constexpr const char* const id_did_you_know = "Did you know?";

void Cool::DidYouKnow::open()
{
    _is_open = true;
    ImGui::OpenPopup(id_did_you_know);
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void Cool::DidYouKnow::open_ifn()
{
    auto const difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (!_is_open && difference > timeToWait)
    {
        _is_open = true;
        ImGui::OpenPopup(id_did_you_know);
        _timestamp_last_opening = std::chrono::steady_clock::now();
    }
}

void Cool::DidYouKnow::prepare_next_tip()
{
    _current_tip_index++;
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
}

void Cool::DidYouKnow::imgui_windows()
{
    if (ImGui::BeginPopupModal(id_did_you_know, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::TextUnformatted(all_tips.at(_current_tip_index));
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();
            prepare_next_tip();
        }

        ImGui::SameLine();

        if (ImGui::Button("Show all tips", ImVec2(120, 0)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();

            _show_all_tips = true;
            prepare_next_tip();
        }

        ImGui::EndPopup();
    }

    if (_show_all_tips)
    {
        ImGui::Begin("All tips", &_show_all_tips);
        Cool::imgui_all_tips();
        ImGui::End();
    }
}

void Cool::imgui_all_tips()
{
    for (auto const& tip : all_tips)
    {
        ImGui::TextUnformatted(tip);

        if (tip != all_tips.back())
        {
            ImGui::Separator();
        }
    }
}