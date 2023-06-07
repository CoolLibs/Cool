#include "TipsManager.h"
#include <imgui.h>
#include <chrono>
#include <string>
#include "Cool/ImGui/markdown.h"
#include "fmt/compile.h"
#include "time_to_wait.h"

namespace Cool {

void TipsManager::open_popup()
{
    prepare_next_tip();
    _window.open();
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void TipsManager::open_ifn()
{
    auto const difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (_app_has_just_been_opened && difference > time_to_wait())
        open_popup();
    _app_has_just_been_opened = false;
}

void TipsManager::prepare_next_tip()
{
    _current_tip_index++;
}

static void imgui_all_tips(Tips all_tips)
{
    for (auto const& tip : all_tips)
    {
        ImGuiExtras::markdown(tip);
        if (&tip != &all_tips.back())
            ImGui::SeparatorText("");
    }
}

auto TipsManager::get_current_tip(Tips all_tips) -> const char*
{
    _current_tip_index = _current_tip_index % all_tips.size();
    return all_tips[_current_tip_index];
}

void TipsManager::imgui_windows(Tips all_tips)
{
    _window.show([&]() {
        ImGuiExtras::markdown(get_current_tip(all_tips));
        ImGui::SeparatorText("");

        auto const button_width = ImGui::GetContentRegionAvail().x / 2.f - ImGui::GetStyle().ItemSpacing.x / 2.f;

        if (ImGui::Button("Got it!", {button_width, 0.f})
            || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
        {
            _window.close();
        }

        ImGui::SameLine();

        if (ImGui::Button("Show me all the tips", {button_width, 0.f}))
        {
            _window.close();
            _show_all_tips = true;
        }
    });

    if (_show_all_tips)
    {
        ImGui::Begin(Cool::icon_fmt("All tips", ICOMOON_BUBBLE).c_str(), &_show_all_tips);
        imgui_all_tips(all_tips);
        ImGui::End();
    }

    open_ifn();
}

} // namespace Cool