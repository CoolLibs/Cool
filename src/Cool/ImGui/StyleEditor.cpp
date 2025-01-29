#include "StyleEditor.h"
#include <imgui-node-editor/imgui_node_editor.h>
#include <imgui.h>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Nodes/nodes_style_editor.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Json.hpp"

namespace ax::NodeEditor {

void to_json(nlohmann::json& json, ax::NodeEditor::Style const& style) // NOLINT(*use-internal-linkage) This function is used by the JsonAutoSerializer
{
#include "generated_style_nodes/json_set.inl"
}

void from_json(nlohmann::json const& json, ax::NodeEditor::Style& style) // NOLINT(*use-internal-linkage) This function is used by the JsonAutoSerializer
{
#include "generated_style_nodes/json_get.inl"
}

} // namespace ax::NodeEditor

void to_json(nlohmann::json& json, ImGuiStyle const& style) // NOLINT(*use-internal-linkage) This function is used by the JsonAutoSerializer
{
#include "generated_style/json_set.inl"
}
void from_json(nlohmann::json const& json, ImGuiStyle& style) // NOLINT(*use-internal-linkage) This function is used by the JsonAutoSerializer
{
#include "generated_style/json_get.inl"
}

namespace Cool {

namespace ImGuiExtras {
void to_json(nlohmann::json& json, ImGuiExtras::Style const& style) // NOLINT(*use-internal-linkage) This function is used by the JsonAutoSerializer
{
#include "generated_style_extras/json_set.inl"
}
void from_json(nlohmann::json const& json, ImGuiExtras::Style& style) // NOLINT(*use-internal-linkage) This function is used by the JsonAutoSerializer
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
    b |= ImGui::SliderFloat("TabBarBorderSize", &ImGui::GetStyle().TabBarBorderSize, 0.0f, 2.0f, "%.0f");
    b |= ImGui::SliderFloat("DockingSplitterSize", &ImGui::GetStyle().DockingSeparatorSize, 0.0f, 12.0f, "%.0f");

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
    b |= ImGui::SliderFloat("LayoutAlign", &ImGui::GetStyle().LayoutAlign, 0.0f, 1.0f);

    ImGuiExtras::separator_text("Tables");
    b |= ImGui::SliderFloat2("CellPadding", (float*)&ImGui::GetStyle().CellPadding, 0.0f, 20.0f, "%.0f");
    b |= ImGui::SliderAngle("TableAngledHeadersAngle", &ImGui::GetStyle().TableAngledHeadersAngle, -50.0f, +50.0f);

    ImGuiExtras::separator_text("Misc");
    b |= ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&ImGui::GetStyle().DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
    ImGuiExtras::help_marker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");

    if (b)
        _serializer.save();
}

} // namespace Cool