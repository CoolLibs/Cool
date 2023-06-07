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

void DidYouKnow::open_popup(Tips all_tips)
{
    prepare_next_tip(all_tips);

    _is_open = true;
    ImGui::OpenPopup(did_you_know_window_title());
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void DidYouKnow::open_ifn(Tips all_tips)
{
    auto const difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (_app_has_just_been_opened && !_is_open && difference > time_to_wait())
    {
        _app_has_just_been_opened = false; // TODO(JF) do it after the if
        open_popup(all_tips);
    }
}

void DidYouKnow::prepare_next_tip(Tips all_tips)
{
    _current_tip_index++;
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
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

void DidYouKnow::imgui_windows(Tips all_tips)
{
    open_ifn(all_tips);

    if (ImGui::BeginPopupModal(did_you_know_window_title(), &_is_open))
    {
        ImGuiExtras::markdown(all_tips[_current_tip_index]);
        ImGui::SeparatorText("");

        if (ImGui::Button("Got it!", {ImGui::GetContentRegionAvail().x / 4.f, 0.f})
            || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            _is_open = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Show me all the tips", {ImGui::GetContentRegionAvail().x / 2.f, 0.f}))
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