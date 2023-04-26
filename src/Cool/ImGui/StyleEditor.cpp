#include "StyleEditor.h"
#include <cereal/archives/json.hpp>
#include "Cool/ImGui/ImGuiExtras.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/Path/Path.h"
#include "Cool/Serialization/Serialization.h"
#include "ImStyleEd/cereal_style.hpp"
#include "imgui-node-editor/imgui_node_editor.h"

namespace cereal {
template<class Archive>
void serialize(Archive& archive, ax::NodeEditor::Style& style)
{
    archive(
        cereal::make_nvp("NodePadding", style.NodePadding),
        cereal::make_nvp("NodeRounding", style.NodeRounding),
        cereal::make_nvp("NodeBorderWidth", style.NodeBorderWidth),
        cereal::make_nvp("HoveredNodeBorderWidth", style.HoveredNodeBorderWidth),
        cereal::make_nvp("SelectedNodeBorderWidth", style.SelectedNodeBorderWidth),
        cereal::make_nvp("PinRounding", style.PinRounding),
        cereal::make_nvp("PinBorderWidth", style.PinBorderWidth),
        cereal::make_nvp("LinkStrength", style.LinkStrength),
        cereal::make_nvp("SourceDirection", style.SourceDirection),
        cereal::make_nvp("TargetDirection", style.TargetDirection),
        cereal::make_nvp("ScrollDuration", style.ScrollDuration),
        cereal::make_nvp("FlowMarkerDistance", style.FlowMarkerDistance),
        cereal::make_nvp("FlowSpeed", style.FlowSpeed),
        cereal::make_nvp("FlowDuration", style.FlowDuration),
        cereal::make_nvp("PivotAlignment", style.PivotAlignment),
        cereal::make_nvp("PivotSize", style.PivotSize),
        cereal::make_nvp("PivotScale", style.PivotScale),
        cereal::make_nvp("PinCorners", style.PinCorners),
        cereal::make_nvp("PinRadius", style.PinRadius),
        cereal::make_nvp("PinArrowSize", style.PinArrowSize),
        cereal::make_nvp("PinArrowWidth", style.PinArrowWidth),
        cereal::make_nvp("GroupRounding", style.GroupRounding),
        cereal::make_nvp("GroupBorderWidth", style.GroupBorderWidth),
        cereal::make_nvp("HighlightConnectedLinks", style.HighlightConnectedLinks),
        cereal::make_nvp("SnapLinkToPinDir", style.SnapLinkToPinDir)
    );
}

} // namespace cereal

namespace Cool {

StyleEditor::StyleEditor()
{
    std::ignore = Cool::Serialization::load<StyleEditor, cereal::JSONInputArchive>(*this, Cool::Path::root() / "style.json");
}

StyleEditor::~StyleEditor()
{
    Cool::Serialization::save<StyleEditor, cereal::JSONOutputArchive>(*this, Cool::Path::root() / "style.json");
}

void StyleEditor::imgui()
{
    ImGui::SeparatorText("Rendering");
    ImGui::SliderFloat("Disabled Alpha", &ImGui::GetStyle().DisabledAlpha, 0.f, 1.f);
    ImGui::SameLine();
    ImGuiExtras::help_marker("Additional alpha multiplier for disabled items (multiply over current value of Alpha).");

    ImGui::SeparatorText("Main");
    ImGui::SliderFloat2("WindowPadding", (float*)&ImGui::GetStyle().WindowPadding, 0.0f, 20.0f, "%.0f");
    ImGuiExtras::GetStyle().imgui_frame_padding();
    ImGuiExtras::GetStyle().imgui_title_bar_padding();
    ImGui::SliderFloat2("CellPadding", (float*)&ImGui::GetStyle().CellPadding, 0.0f, 20.0f, "%.0f");
    ImGui::SliderFloat2("ItemSpacing", (float*)&ImGui::GetStyle().ItemSpacing, 0.0f, 20.0f, "%.0f");
    ImGui::SliderFloat2("ItemInnerSpacing", (float*)&ImGui::GetStyle().ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
    ImGuiExtras::GetStyle().imgui_floating_buttons_spacing();
    ImGui::SliderFloat2("TouchExtraPadding", (float*)&ImGui::GetStyle().TouchExtraPadding, 0.0f, 10.0f, "%.0f");
    ImGui::SliderFloat("IndentSpacing", &ImGui::GetStyle().IndentSpacing, 0.0f, 30.0f, "%.0f");
    ImGui::SliderFloat("ScrollbarSize", &ImGui::GetStyle().ScrollbarSize, 1.0f, 20.0f, "%.0f");
    ImGui::SliderFloat("GrabMinSize", &ImGui::GetStyle().GrabMinSize, 1.0f, 20.0f, "%.0f");

    ImGui::SeparatorText("Borders");
    ImGui::SliderFloat("WindowBorderSize", &ImGui::GetStyle().WindowBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("ChildBorderSize", &ImGui::GetStyle().ChildBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("PopupBorderSize", &ImGui::GetStyle().PopupBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("FrameBorderSize", &ImGui::GetStyle().FrameBorderSize, 0.0f, 1.0f, "%.0f");
    ImGui::SliderFloat("TabBorderSize", &ImGui::GetStyle().TabBorderSize, 0.0f, 1.0f, "%.0f");

    ImGui::SeparatorText("Rounding");
    ImGui::SliderFloat("WindowRounding", &ImGui::GetStyle().WindowRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("ChildRounding", &ImGui::GetStyle().ChildRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("FrameRounding", &ImGui::GetStyle().FrameRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("PopupRounding", &ImGui::GetStyle().PopupRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("ScrollbarRounding", &ImGui::GetStyle().ScrollbarRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("GrabRounding", &ImGui::GetStyle().GrabRounding, 0.0f, 12.0f, "%.0f");
    ImGui::SliderFloat("TabRounding", &ImGui::GetStyle().TabRounding, 0.0f, 12.0f, "%.0f");

    ImGui::SeparatorText("Widgets");
    ImGui::SliderFloat2("WindowTitleAlign", (float*)&ImGui::GetStyle().WindowTitleAlign, 0.0f, 1.0f, "%.2f");
    int window_menu_button_position = ImGui::GetStyle().WindowMenuButtonPosition + 1;
    if (ImGui::Combo("WindowMenuButtonPosition", (int*)&window_menu_button_position, "None\0Left\0Right\0"))
        ImGui::GetStyle().WindowMenuButtonPosition = window_menu_button_position - 1;
    ImGui::Combo("ColorButtonPosition", (int*)&ImGui::GetStyle().ColorButtonPosition, "Left\0Right\0");
    ImGui::SliderFloat2("ButtonTextAlign", (float*)&ImGui::GetStyle().ButtonTextAlign, 0.0f, 1.0f, "%.2f");
    ImGui::SameLine();
    ImGuiExtras::help_marker("Alignment applies when a button is larger than its text content.");
    ImGui::SliderFloat2("SelectableTextAlign", (float*)&ImGui::GetStyle().SelectableTextAlign, 0.0f, 1.0f, "%.2f");
    ImGui::SameLine();
    ImGuiExtras::help_marker("Alignment applies when a selectable is larger than its text content.");
    ImGui::SliderFloat("SeparatorTextBorderSize", &ImGui::GetStyle().SeparatorTextBorderSize, 0.0f, 10.0f, "%.0f");
    ImGui::SliderFloat2("SeparatorTextAlign", (float*)&ImGui::GetStyle().SeparatorTextAlign, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat2("SeparatorTextPadding", (float*)&ImGui::GetStyle().SeparatorTextPadding, 0.0f, 40.0f, "%0.f");
    ImGui::SliderFloat("LogSliderDeadzone", &ImGui::GetStyle().LogSliderDeadzone, 0.0f, 12.0f, "%.0f");

    ImGui::SeparatorText("Misc");
    ImGui::SliderFloat2("DisplaySafeAreaPadding", (float*)&ImGui::GetStyle().DisplaySafeAreaPadding, 0.0f, 30.0f, "%.0f");
    ImGui::SameLine();
    ImGuiExtras::help_marker("Adjust if you cannot see the edges of your screen (e.g. on a TV where scaling has not been configured).");
}

} // namespace Cool