#include "DidYouKnow.h"
#include <imgui.h>
#include <chrono>
#include <string>
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/ImGui/markdown.h"
#include "fmt/compile.h"
#include "time_to_wait.h"

namespace Cool {

static auto did_you_know_window_title() -> const char*
{
    static std::string const title = Cool::icon_fmt("Did you know?", ICOMOON_BUBBLE);
    return title.c_str();
}

void DidYouKnow::open_popup()
{
    prepare_next_tip();

    _is_open = true;
    ImGui::OpenPopup(did_you_know_window_title());
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void DidYouKnow::open_ifn()
{
    auto const difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (_app_has_just_been_opened && !_is_open && difference > time_to_wait())
    {
        open_popup();
    }
    _app_has_just_been_opened = false;
}

void DidYouKnow::prepare_next_tip()
{
    _current_tip_index++;
}

static void imgui_all_tips(Tips all_tips)
{
    for (auto const& tip : all_tips)
    {
        ImGuiExtras::markdown(tip);
        if (&tip != &all_tips.back())
            ImGui::Separator();
    }
}

auto DidYouKnow::get_current_tip(Tips all_tips) -> const char*
{
    _current_tip_index = _current_tip_index % all_tips.size();
    return all_tips[_current_tip_index];
}

void DidYouKnow::imgui_windows(Tips all_tips)
{
    open_ifn();

    if (ImGui::BeginPopupModal(did_you_know_window_title(), &_is_open))
    {
        ImGuiExtras::markdown(get_current_tip(all_tips));
        ImGui::SeparatorText("");

        auto const button_width = ImGui::GetContentRegionAvail().x / 2.f - ImGui::GetStyle().ItemSpacing.x / 2.f;

        if (ImGui::Button("Got it!", {button_width, 0.f})
            || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Show me all the tips", {button_width, 0.f}))
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

} // namespace Cool