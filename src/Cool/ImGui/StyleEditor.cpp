#include "StyleEditor.h"
#include <imgui-node-editor/imgui_node_editor.h>
#include <imgui.h>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Nodes/nodes_style_editor.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Json.hpp"

namespace ax::NodeEditor {

static void to_json(nlohmann::json& json, ax::NodeEditor::Style const& style)
{
    Cool::json_set(json, "NodePadding", style.NodePadding);
    Cool::json_set(json, "NodeRounding", style.NodeRounding);
    Cool::json_set(json, "NodeBorderWidth", style.NodeBorderWidth);
    Cool::json_set(json, "HoveredNodeBorderWidth", style.HoveredNodeBorderWidth);
    Cool::json_set(json, "SelectedNodeBorderWidth", style.SelectedNodeBorderWidth);
    Cool::json_set(json, "PinRounding", style.PinRounding);
    Cool::json_set(json, "PinBorderWidth", style.PinBorderWidth);
    Cool::json_set(json, "LinkStrength", style.LinkStrength);
    Cool::json_set(json, "SourceDirection", style.SourceDirection);
    Cool::json_set(json, "TargetDirection", style.TargetDirection);
    Cool::json_set(json, "ScrollDuration", style.ScrollDuration);
    Cool::json_set(json, "FlowMarkerDistance", style.FlowMarkerDistance);
    Cool::json_set(json, "FlowSpeed", style.FlowSpeed);
    Cool::json_set(json, "FlowDuration", style.FlowDuration);
    Cool::json_set(json, "PivotAlignment", style.PivotAlignment);
    Cool::json_set(json, "PivotSize", style.PivotSize);
    Cool::json_set(json, "PivotScale", style.PivotScale);
    Cool::json_set(json, "PinCorners", style.PinCorners);
    Cool::json_set(json, "PinRadius", style.PinRadius);
    Cool::json_set(json, "PinArrowSize", style.PinArrowSize);
    Cool::json_set(json, "PinArrowWidth", style.PinArrowWidth);
    Cool::json_set(json, "GroupRounding", style.GroupRounding);
    Cool::json_set(json, "GroupBorderWidth", style.GroupBorderWidth);
    Cool::json_set(json, "HighlightConnectedLinks", style.HighlightConnectedLinks);
    Cool::json_set(json, "SnapLinkToPinDir", style.SnapLinkToPinDir);
}

static void from_json(nlohmann::json const& json, ax::NodeEditor::Style& style)
{
    Cool::json_get(json, "NodePadding", style.NodePadding);
    Cool::json_get(json, "NodeRounding", style.NodeRounding);
    Cool::json_get(json, "NodeBorderWidth", style.NodeBorderWidth);
    Cool::json_get(json, "HoveredNodeBorderWidth", style.HoveredNodeBorderWidth);
    Cool::json_get(json, "SelectedNodeBorderWidth", style.SelectedNodeBorderWidth);
    Cool::json_get(json, "PinRounding", style.PinRounding);
    Cool::json_get(json, "PinBorderWidth", style.PinBorderWidth);
    Cool::json_get(json, "LinkStrength", style.LinkStrength);
    Cool::json_get(json, "SourceDirection", style.SourceDirection);
    Cool::json_get(json, "TargetDirection", style.TargetDirection);
    Cool::json_get(json, "ScrollDuration", style.ScrollDuration);
    Cool::json_get(json, "FlowMarkerDistance", style.FlowMarkerDistance);
    Cool::json_get(json, "FlowSpeed", style.FlowSpeed);
    Cool::json_get(json, "FlowDuration", style.FlowDuration);
    Cool::json_get(json, "PivotAlignment", style.PivotAlignment);
    Cool::json_get(json, "PivotSize", style.PivotSize);
    Cool::json_get(json, "PivotScale", style.PivotScale);
    Cool::json_get(json, "PinCorners", style.PinCorners);
    Cool::json_get(json, "PinRadius", style.PinRadius);
    Cool::json_get(json, "PinArrowSize", style.PinArrowSize);
    Cool::json_get(json, "PinArrowWidth", style.PinArrowWidth);
    Cool::json_get(json, "GroupRounding", style.GroupRounding);
    Cool::json_get(json, "GroupBorderWidth", style.GroupBorderWidth);
    Cool::json_get(json, "HighlightConnectedLinks", style.HighlightConnectedLinks);
    Cool::json_get(json, "SnapLinkToPinDir", style.SnapLinkToPinDir);
}

} // namespace ax::NodeEditor

static void to_json(nlohmann::json& json, ImGuiStyle const& style)
{
    Cool::json_set(json, "Alpha", style.Alpha);
    Cool::json_set(json, "DisabledAlpha", style.DisabledAlpha);
    Cool::json_set(json, "WindowPadding", style.WindowPadding);
    Cool::json_set(json, "WindowRounding", style.WindowRounding);
    Cool::json_set(json, "WindowBorderSize", style.WindowBorderSize);
    Cool::json_set(json, "WindowMinSize", style.WindowMinSize);
    Cool::json_set(json, "WindowTitleAlign", style.WindowTitleAlign);
    Cool::json_set(json, "WindowMenuButtonPosition", style.WindowMenuButtonPosition);
    Cool::json_set(json, "ChildRounding", style.ChildRounding);
    Cool::json_set(json, "ChildBorderSize", style.ChildBorderSize);
    Cool::json_set(json, "PopupRounding", style.PopupRounding);
    Cool::json_set(json, "PopupBorderSize", style.PopupBorderSize);
    Cool::json_set(json, "FramePadding", style.FramePadding);
    Cool::json_set(json, "FrameRounding", style.FrameRounding);
    Cool::json_set(json, "FrameBorderSize", style.FrameBorderSize);
    Cool::json_set(json, "ItemSpacing", style.ItemSpacing);
    Cool::json_set(json, "ItemInnerSpacing", style.ItemInnerSpacing);
    Cool::json_set(json, "CellPadding", style.CellPadding);
    Cool::json_set(json, "TouchExtraPadding", style.TouchExtraPadding);
    Cool::json_set(json, "IndentSpacing", style.IndentSpacing);
    Cool::json_set(json, "ColumnsMinSpacing", style.ColumnsMinSpacing);
    Cool::json_set(json, "ScrollbarSize", style.ScrollbarSize);
    Cool::json_set(json, "ScrollbarRounding", style.ScrollbarRounding);
    Cool::json_set(json, "GrabMinSize", style.GrabMinSize);
    Cool::json_set(json, "GrabRounding", style.GrabRounding);
    Cool::json_set(json, "LogSliderDeadzone", style.LogSliderDeadzone);
    Cool::json_set(json, "TabRounding", style.TabRounding);
    Cool::json_set(json, "TabBorderSize", style.TabBorderSize);
    Cool::json_set(json, "TabMinWidthForCloseButton", style.TabMinWidthForCloseButton);
    Cool::json_set(json, "ColorButtonPosition", style.ColorButtonPosition);
    Cool::json_set(json, "ButtonTextAlign", style.ButtonTextAlign);
    Cool::json_set(json, "SelectableTextAlign", style.SelectableTextAlign);
    Cool::json_set(json, "SeparatorTextBorderSize", style.SeparatorTextBorderSize);
    Cool::json_set(json, "SeparatorTextAlign", style.SeparatorTextAlign);
    Cool::json_set(json, "SeparatorTextPadding", style.SeparatorTextPadding);
    Cool::json_set(json, "DisplayWindowPadding", style.DisplayWindowPadding);
    Cool::json_set(json, "DisplaySafeAreaPadding", style.DisplaySafeAreaPadding);
    Cool::json_set(json, "MouseCursorScale", style.MouseCursorScale);
    Cool::json_set(json, "AntiAliasedLines", style.AntiAliasedLines);
    Cool::json_set(json, "AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex);
    Cool::json_set(json, "AntiAliasedFill", style.AntiAliasedFill);
    Cool::json_set(json, "CurveTessellationTol", style.CurveTessellationTol);
    Cool::json_set(json, "CircleTessellationMaxError", style.CircleTessellationMaxError);
    Cool::json_set(json, "HoverFlagsForTooltipMouse", style.HoverFlagsForTooltipMouse);
    Cool::json_set(json, "HoverFlagsForTooltipNav", style.HoverFlagsForTooltipNav);
    Cool::json_set(json, "HoverDelayShort", style.HoverDelayShort);
    Cool::json_set(json, "HoverDelayNormal", style.HoverDelayNormal);
    Cool::json_set(json, "HoverStationaryDelay", style.HoverStationaryDelay);
}
static void from_json(nlohmann::json const& json, ImGuiStyle& style)
{
    Cool::json_get(json, "Alpha", style.Alpha);
    Cool::json_get(json, "DisabledAlpha", style.DisabledAlpha);
    Cool::json_get(json, "WindowPadding", style.WindowPadding);
    Cool::json_get(json, "WindowRounding", style.WindowRounding);
    Cool::json_get(json, "WindowBorderSize", style.WindowBorderSize);
    Cool::json_get(json, "WindowMinSize", style.WindowMinSize);
    Cool::json_get(json, "WindowTitleAlign", style.WindowTitleAlign);
    Cool::json_get(json, "WindowMenuButtonPosition", style.WindowMenuButtonPosition);
    Cool::json_get(json, "ChildRounding", style.ChildRounding);
    Cool::json_get(json, "ChildBorderSize", style.ChildBorderSize);
    Cool::json_get(json, "PopupRounding", style.PopupRounding);
    Cool::json_get(json, "PopupBorderSize", style.PopupBorderSize);
    Cool::json_get(json, "FramePadding", style.FramePadding);
    Cool::json_get(json, "FrameRounding", style.FrameRounding);
    Cool::json_get(json, "FrameBorderSize", style.FrameBorderSize);
    Cool::json_get(json, "ItemSpacing", style.ItemSpacing);
    Cool::json_get(json, "ItemInnerSpacing", style.ItemInnerSpacing);
    Cool::json_get(json, "CellPadding", style.CellPadding);
    Cool::json_get(json, "TouchExtraPadding", style.TouchExtraPadding);
    Cool::json_get(json, "IndentSpacing", style.IndentSpacing);
    Cool::json_get(json, "ColumnsMinSpacing", style.ColumnsMinSpacing);
    Cool::json_get(json, "ScrollbarSize", style.ScrollbarSize);
    Cool::json_get(json, "ScrollbarRounding", style.ScrollbarRounding);
    Cool::json_get(json, "GrabMinSize", style.GrabMinSize);
    Cool::json_get(json, "GrabRounding", style.GrabRounding);
    Cool::json_get(json, "LogSliderDeadzone", style.LogSliderDeadzone);
    Cool::json_get(json, "TabRounding", style.TabRounding);
    Cool::json_get(json, "TabBorderSize", style.TabBorderSize);
    Cool::json_get(json, "TabMinWidthForCloseButton", style.TabMinWidthForCloseButton);
    Cool::json_get(json, "ColorButtonPosition", style.ColorButtonPosition);
    Cool::json_get(json, "ButtonTextAlign", style.ButtonTextAlign);
    Cool::json_get(json, "SelectableTextAlign", style.SelectableTextAlign);
    Cool::json_get(json, "SeparatorTextBorderSize", style.SeparatorTextBorderSize);
    Cool::json_get(json, "SeparatorTextAlign", style.SeparatorTextAlign);
    Cool::json_get(json, "SeparatorTextPadding", style.SeparatorTextPadding);
    Cool::json_get(json, "DisplayWindowPadding", style.DisplayWindowPadding);
    Cool::json_get(json, "DisplaySafeAreaPadding", style.DisplaySafeAreaPadding);
    Cool::json_get(json, "MouseCursorScale", style.MouseCursorScale);
    Cool::json_get(json, "AntiAliasedLines", style.AntiAliasedLines);
    Cool::json_get(json, "AntiAliasedLinesUseTex", style.AntiAliasedLinesUseTex);
    Cool::json_get(json, "AntiAliasedFill", style.AntiAliasedFill);
    Cool::json_get(json, "CurveTessellationTol", style.CurveTessellationTol);
    Cool::json_get(json, "CircleTessellationMaxError", style.CircleTessellationMaxError);
    Cool::json_get(json, "HoverFlagsForTooltipMouse", style.HoverFlagsForTooltipMouse);
    Cool::json_get(json, "HoverFlagsForTooltipNav", style.HoverFlagsForTooltipNav);
    Cool::json_get(json, "HoverDelayShort", style.HoverDelayShort);
    Cool::json_get(json, "HoverDelayNormal", style.HoverDelayNormal);
    Cool::json_get(json, "HoverStationaryDelay", style.HoverStationaryDelay);
}

namespace Cool {

namespace ImGuiExtras {
static void to_json(nlohmann::json& json, ImGuiExtras::Style const& style)
{
#include "generated_style_extras/json_set.inl"
}
static void from_json(nlohmann::json const& json, ImGuiExtras::Style& style)
{
#include "generated_style_extras/json_get.inl"
}
} // namespace ImGuiExtras

StyleEditor::StyleEditor()
    : _serializer{
          "style.json",
          [](nlohmann::json const& json) {
              json_get(json, "ImGui", ImGui::GetStyle());
              json_get(json, "ImGuiExtras", ImGuiExtras::GetStyle());
              json_get(json, "ImNodes", ax::NodeEditor::GetStyle());
          },
          [](nlohmann::json& json) {
              json_set(json, "ImGui", ImGui::GetStyle());
              json_set(json, "ImGuiExtras", ImGuiExtras::GetStyle());
              json_set(json, "ImNodes", ax::NodeEditor::GetStyle());
          }
      }
{}

void StyleEditor::imgui()
{
    bool b = false;
    ImGuiExtras::separator_text("Rendering");
    b |= ImGui::SliderFloat("Disabled Alpha", &ImGui::GetStyle().DisabledAlpha, 0.f, 1.f);
    ImGuiExtras::help_marker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");

    ImGuiExtras::separator_text("Main");
    b |= ImGui::SliderFloat2("WindowPadding", (float*)&ImGui::GetStyle().WindowPadding, 0.0f, 20.0f, "%.0f");
    b |= ImGuiExtras::GetStyle().imgui_frame_padding();
    b |= ImGuiExtras::GetStyle().imgui_tab_bar_padding();
    b |= ImGuiExtras::GetStyle().imgui_menu_bar_spacing();
    b |= ImGui::SliderFloat2("CellPadding", (float*)&ImGui::GetStyle().CellPadding, 0.0f, 20.0f, "%.0f");
    b |= ImGui::SliderFloat2("ItemSpacing", (float*)&ImGui::GetStyle().ItemSpacing, 0.0f, 20.0f, "%.0f");
    b |= ImGui::SliderFloat2("ItemInnerSpacing", (float*)&ImGui::GetStyle().ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
    b |= ImGuiExtras::GetStyle().imgui_floating_buttons_spacing();
    b |= ImGui::SliderFloat2("TouchExtraPadding", (float*)&ImGui::GetStyle().TouchExtraPadding, 0.0f, 10.0f, "%.0f");
    b |= ImGui::SliderFloat("IndentSpacing", &ImGui::GetStyle().IndentSpacing, 0.0f, 30.0f, "%.0f");
    b |= ImGui::SliderFloat("ScrollbarSize", &ImGui::GetStyle().ScrollbarSize, 1.0f, 20.0f, "%.0f");
    b |= ImGui::SliderFloat("GrabMinSize", &ImGui::GetStyle().GrabMinSize, 1.0f, 20.0f, "%.0f");

    ImGuiExtras::separator_text("Borders");
    b |= ImGui::SliderFloat("WindowBorderSize", &ImGui::GetStyle().WindowBorderSize, 0.0f, 1.0f, "%.0f");
    b |= ImGui::SliderFloat("ChildBorderSize", &ImGui::GetStyle().ChildBorderSize, 0.0f, 1.0f, "%.0f");
    b |= ImGui::SliderFloat("PopupBorderSize", &ImGui::GetStyle().PopupBorderSize, 0.0f, 1.0f, "%.0f");
    b |= ImGui::SliderFloat("FrameBorderSize", &ImGui::GetStyle().FrameBorderSize, 0.0f, 1.0f, "%.0f");
    b |= ImGui::SliderFloat("TabBorderSize", &ImGui::GetStyle().TabBorderSize, 0.0f, 1.0f, "%.0f");

    ImGuiExtras::separator_text("Rounding");
    b |= ImGui::SliderFloat("WindowRounding", &ImGui::GetStyle().WindowRounding, 0.0f, 12.0f, "%.0f");
    b |= ImGui::SliderFloat("ChildRounding", &ImGui::GetStyle().ChildRounding, 0.0f, 12.0f, "%.0f");
    b |= ImGui::SliderFloat("FrameRounding", &ImGui::GetStyle().FrameRounding, 0.0f, 12.0f, "%.0f");
    b |= ImGui::SliderFloat("PopupRounding", &ImGui::GetStyle().PopupRounding, 0.0f, 12.0f, "%.0f");
    b |= ImGui::SliderFloat("ScrollbarRounding", &ImGui::GetStyle().ScrollbarRounding, 0.0f, 12.0f, "%.0f");
    b |= ImGui::SliderFloat("GrabRounding", &ImGui::GetStyle().GrabRounding, 0.0f, 12.0f, "%.0f");
    b |= ImGui::SliderFloat("TabRounding", &ImGui::GetStyle().TabRounding, 0.0f, 12.0f, "%.0f");

    ImGuiExtras::separator_text("Widgets");
    b |= ImGui::SliderFloat2("WindowTitleAlign", (float*)&ImGui::GetStyle().WindowTitleAlign, 0.0f, 1.0f, "%.2f");
    int window_menu_button_position = ImGui::GetStyle().WindowMenuButtonPosition + 1;
    if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
    {
        ImGui::GetStyle().WindowMenuButtonPosition = window_menu_button_position - 1;
        b                                          = true;
    }
    b |= ImGui::Combo("ColorButtonPosition", (int*)&ImGui::GetStyle().ColorButtonPosition, "Left\0Right\0");
    b |= ImGui::SliderFloat2("ButtonTextAlign", (float*)&ImGui::GetStyle().ButtonTextAlign, 0.0f, 1.0f, "%.2f");
    ImGuiExtras::help_marker("Alignment applies when a button is larger than its text content.");
    b |= ImGui::SliderFloat2("SelectableTextAlign", (float*)&ImGui::GetStyle().SelectableTextAlign, 0.0f, 1.0f, "%.2f");
    ImGuiExtras::help_marker("Alignment applies when a selectable is larger than its text content.");
    b |= ImGui::SliderFloat("SeparatorTextBorderSize", &ImGui::GetStyle().SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
    b |= ImGui::SliderFloat2("SeparatorTextAlign", (float*)&ImGui::GetStyle().SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
    b |= ImGui::SliderFloat2("SeparatorTextPadding", (float*)&ImGui::GetStyle().SeparatorTextPadding, 0.0f, 40.0f, "%0.f");
    b |= ImGui::SliderFloat("LogSliderDeadzone", &ImGui::GetStyle().LogSliderDeadzone, 0.0f, 12.0f, "%.0f");

    ImGui::SeparatorText("Tooltips");
    for (int n = 0; n < 2; n++)
    {
        if (ImGui::TreeNodeEx(n == 0 ? "HoverFlagsForTooltipMouse" : "HoverFlagsForTooltipNav"))
        {
            ImGuiHoveredFlags* p = (n == 0) ? &ImGui::GetStyle().HoverFlagsForTooltipMouse : &ImGui::GetStyle().HoverFlagsForTooltipNav;
            b |= ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayNone", p, ImGuiHoveredFlags_DelayNone);
            b |= ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayShort", p, ImGuiHoveredFlags_DelayShort);
            b |= ImGui::CheckboxFlags("ImGuiHoveredFlags_DelayNormal", p, ImGuiHoveredFlags_DelayNormal);
            b |= ImGui::CheckboxFlags("ImGuiHoveredFlags_Stationary", p, ImGuiHoveredFlags_Stationary);
            b |= ImGui::CheckboxFlags("ImGuiHoveredFlags_NoSharedDelay", p, ImGuiHoveredFlags_NoSharedDelay);
            ImGui::TreePop();
        }
    }
    b |= ImGui::InputFloat("Hover delay short", &ImGui::GetStyle().HoverDelayShort);
    b |= ImGui::InputFloat("Hover delay normal", &ImGui::GetStyle().HoverDelayNormal);
    b |= ImGui::InputFloat("Hover stationary delay", &ImGui::GetStyle().HoverStationaryDelay);

    ImGuiExtras::separator_text("Nodes");
    b |= nodes_style_editor();

    ImGuiExtras::separator_text("Misc");
    b |= ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&ImGui::GetStyle().DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
    ImGuiExtras::help_marker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");

    if (b)
        _serializer.save();
}

} // namespace Cool