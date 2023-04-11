#include "didYouKnowModal.hpp"
#include <imgui.h>
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include "Cool/ImGui/markdown.h"
#include "fmt/compile.h"

static constexpr const char* const id_did_you_know = "Did you know?";

auto timeToWait() -> std::chrono::steady_clock::duration
{
    return 1h;
}

void Cool::DidYouKnow::open(std::span<const char* const>& all_tips)
{
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
    _is_open = true;
    ImGui::OpenPopup(id_did_you_know);
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void Cool::DidYouKnow::open_ifn(std::span<const char* const>& all_tips)
{
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
    auto const difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (_open_of_coollab && !_is_open && difference > timeToWait())
    {
        _open_of_coollab = false;
        _is_open         = true;
        ImGui::OpenPopup(id_did_you_know);
        _timestamp_last_opening = std::chrono::steady_clock::now();
    }
}

void Cool::DidYouKnow::prepare_next_tip(std::span<const char* const>& all_tips)
{
    _current_tip_index++;
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
}

void Cool::DidYouKnow::imgui_windows(std::span<const char* const>& all_tips)
{
    if (ImGui::BeginPopupModal(id_did_you_know, &_is_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        Cool::ImGuiExtras::markdown(all_tips[_current_tip_index]);
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();
            prepare_next_tip(all_tips);
        }

        ImGui::SameLine();

        if (ImGui::Button("Show all tips", ImVec2(120, 0)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();

            _show_all_tips = true;
            prepare_next_tip(all_tips);
        }

        ImGui::EndPopup();
    }

    if (_show_all_tips)
    {
        ImGui::Begin("All tips", &_show_all_tips);
        Cool::imgui_all_tips(all_tips);
        ImGui::End();
    }
}

void Cool::imgui_all_tips(std::span<const char* const>& all_tips)
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

void Cool::update(Cool::DidYouKnow& _did_you_know, std::span<const char* const>& all_tips)
{
    _did_you_know.open_ifn(all_tips);

    _did_you_know.imgui_windows(all_tips);
}