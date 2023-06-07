#include "DidYouKnow.h"
#include <imgui.h>
#include <chrono>
#include <string>
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/ImGui/markdown.h"
#include "fmt/compile.h"
#include "time_to_wait.h"

static auto did_you_know_window_title() -> const char*
{
    static std::string const title = Cool::icon_fmt("Did you know?", ICOMOON_BUBBLE);
    return title.c_str();
}

void Cool::DidYouKnow::open_popup(std::span<const char* const> all_tips)
{
    prepare_next_tip(all_tips);

    _is_open = true;
    ImGui::OpenPopup(did_you_know_window_title());
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void Cool::DidYouKnow::open_ifn(std::span<const char* const> all_tips)
{
    auto const difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (_app_has_just_been_opened && !_is_open && difference > time_to_wait())
    {
        _app_has_just_been_opened = false;

        open_popup(all_tips);
    }
}

void Cool::DidYouKnow::prepare_next_tip(std::span<const char* const> all_tips)
{
    _current_tip_index++;
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
}

static void imgui_all_tips(std::span<const char* const> all_tips)
{
    for (auto const& tip : all_tips)
    {
        ImGui::TextUnformatted(tip);

        if (&tip != &all_tips.back())
        {
            ImGui::Separator();
        }
    }
}

void Cool::DidYouKnow::imgui_windows(std::span<const char* const> all_tips)
{
    open_ifn(all_tips);

    if (ImGui::BeginPopupModal(did_you_know_window_title(), &_is_open, 0))
    {
        Cool::ImGuiExtras::markdown(all_tips[_current_tip_index]);
        ImGui::Separator();

        if (ImGui::Button("Got it!", ImVec2(ImGui::GetContentRegionAvail().x / 4, 0)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Show me all the tips", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();

            _show_all_tips = true;
        }

        ImGui::EndPopup();
    }

    if (_show_all_tips)
    {
        ImGui::Begin(Cool::icon_fmt("All tips", ICOMOON_BUBBLE).c_str(), &_show_all_tips);
        imgui_all_tips(all_tips);
        ImGui::End();
    }
}