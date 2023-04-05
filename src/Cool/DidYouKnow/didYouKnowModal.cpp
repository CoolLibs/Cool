#include "didYouKnowModal.hpp"
#include <imgui.h>
#include <array>
#include <chrono>
#include <string>
#include "fmt/compile.h"

const std::array<const char*, 12> allTips = {
    "You can hold SHIFT to disable docking. Useful when you try to move a window around and docking gets in your way.",
    "The undo-history (CTRL+Z) of your modifications is preserved even when you close and re-open CoolLab! You can control its size in 'Settings > History Size'",
    "Talk about TDR to avoid crashes (https://www.artstation.com/blogs/sebastianbracht/ovQg/workaround-for-the-windows-tdr-crash-issue-while-using-substance-painter, https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&ved=2ahUKEwiV6LmO27_9AhXmcaQEHYw5AdUQFnoECBMQAQ&url=https%3A%2F%2Fsubstance3d.adobe.com%2Fdocumentation%2Fspdoc%2Fgpu-drivers-crash-with-long-computations-tdr-crash-128745489.html&usg=AOvVaw3NUHI7sPGmV__7gxQR4w4U)",
    "Drag from window menu button (upper-left button) to undock an entire node (all windows).",
    "Ctrl+click on a slider to input a value manually (and it also allows you to bypass the bounds of the slider !)",
    "Double-click on title bar to collapse window.",
    "Double-click on lower right corner to auto fit window to its contents.",
    "TAB/SHIFT+TAB to cycle through keyboard editable fields.",
    "Text editor: \n Hold SHIFT or use mouse to select text.\nCTRL+Left/Right to word jump.\nCTRL+Shift+Left/Right to select words.\nCTRL+A or Double-Click to select all.\nCTRL+X,CTRL+C,CTRL+V to use OS clipboard/\nCTRL+Z,CTRL+Y to undo/redo.",
    "ESCAPE to revert text to its original value.",
    "Double-click on title bar to collapse window.",
    "CTRL+Tab to select a window. ImGui::BulletText('While inputing text:\n'); ImGui::Indent(); ImGui::BulletText('CTRL+Left/Right to word jump.'); ImGui::BulletText('CTRL+A or double-click to select all.'); ImGui::BulletText('CTRL+X/C/V to use clipboard cut/copy/paste.'); ImGui::BulletText('CTRL+Z,CTRL+Y to undo/redo.'); ImGui::BulletText('ESCAPE to revert.'); ImGui::Unindent(); ImGui::BulletText('With keyboard navigation enabled:'); ImGui::Indent(); ImGui::BulletText('Arrow keys to navigate.'); ImGui::BulletText('Space or enter to activate a widget.'); ImGui::BulletText('Return to input text into a widget.'); ImGui::BulletText('Escape to deactivate a widget, close popup, exit child window.'); ImGui::BulletText('Alt to jump to the menu layer of a window.');"};

const char* const idDidYouKnow = "Did you know?";

const std::chrono::hours timeToWait = 1h;

void DidYouKnowModal::open()
{
    const auto difference = std::chrono::steady_clock::now() - _timestamp_last_opening;

    if (!_has_been_opened && difference > timeToWait)
    {
        _has_been_opened = true;
        ImGui::OpenPopup(idDidYouKnow);
        _timestamp_last_opening = std::chrono::steady_clock::now();
        return;
    }
}

void DidYouKnowModal::prepare_next_tip()
{
    _current_tip_index++;
    if (_current_tip_index >= allTips.size())
        _current_tip_index = 0;
}

void DidYouKnowModal::imgui_window()
{
    ImGui::Text("%s", allTips.at(_current_tip_index));
    ImGui::Separator();

    if (ImGui::Button("OK", ImVec2(120, 0)))
    {
        ImGui::CloseCurrentPopup();
        prepare_next_tip();
    }

    ImGui::SameLine();

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
        for (auto const& tip : allTips)
        {
            ImGui::Text("%s", tip);
            ImGui::Separator();
        }
        if (ImGui::Button("Got it!"))
            ImGui::CloseCurrentPopup();
        ImGui::EndPopup();
    }
}

void test_did_you_know(DidYouKnowModal& _did_you_know)
{
    _did_you_know.open();

    if (ImGui::BeginPopupModal(idDidYouKnow, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        _did_you_know.imgui_window();
    }
}

void debug_did_you_know(DidYouKnowModal& _did_you_know)
{
    if (ImGui::Button("Test DidYouKnow"))
    {
        _did_you_know._has_been_opened        = false;
        _did_you_know._timestamp_last_opening = std::chrono::steady_clock::now() - std::chrono::hours(2);
    }

    // imgui text with difference between current timestamp of did you know and current timestamp of now
    ImGui::Text("%s", fmt::format("Difference: {}", (std::chrono::steady_clock::now() - _did_you_know._timestamp_last_opening).count()).c_str());

    // imgui text of current timestamp of did you know
    ImGui::Text("%s", fmt::format("Current timestamp: {} s", _did_you_know._timestamp_last_opening.time_since_epoch().count()).c_str());

    // imgui text to precise how much time is needed to wait before showing the did you know again
    ImGui::Text("Time to wait: %s h", std::to_string(timeToWait.count()).c_str());
}