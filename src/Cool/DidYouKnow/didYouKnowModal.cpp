#include "didYouKnowModal.hpp"
#include <imgui.h>
#include <array>
#include <chrono>
#include <iostream>
#include <string>
#include "fmt/compile.h"

static constexpr std::array all_tips = {
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

void DidYouKnowModal::open()
{
    _has_been_opened = true;
    ImGui::OpenPopup(idDidYouKnow);
    _timestamp_last_opening = std::chrono::steady_clock::now();
}

void DidYouKnowModal::open_ifn()
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
    if (_current_tip_index >= all_tips.size())
        _current_tip_index = 0;
}

void DidYouKnowModal::imgui_window()
{
    if (ImGui::BeginPopupModal(idDidYouKnow, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("%s", all_tips.at(_current_tip_index));
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();
            prepare_next_tip();
        }

        ImGui::SameLine();

        if (ImGui::Button("Show all tips", ImVec2(120, 0)))
        {
            ImGui::CloseCurrentPopup();

            _show_all_tips = true;
        }

        ImGui::EndPopup();
    }

    if (_show_all_tips)
    {
        ImGui::Begin("All tips");
        display_all_tips();
        ImGui::End();
    }
}

void DidYouKnowModal::display_all_tips()
{
    // if (ImGui::BeginPopupModal("All tips", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    // {
    for (auto const& tip : all_tips)
    {
        ImGui::Text("%s", tip);
        ImGui::Separator();
    }
    // if (ImGui::Button("Got it!"))
    //     ImGui::CloseCurrentPopup();
    // ImGui::EndPopup();
    // }
}