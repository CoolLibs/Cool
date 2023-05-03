#include "nodes_style_editor.h"
#include "imgui-node-editor/imgui_node_editor.h"

namespace Cool {

void nodes_style_editor()
{
    auto& style = ax::NodeEditor::GetStyle();
    ImGui::Spacing();
    ImGui::DragFloat4("Node Padding", &style.NodePadding.x, 0.1f, 0.0f, 40.0f);
    ImGui::DragFloat("Node Rounding", &style.NodeRounding, 0.1f, 0.0f, 40.0f);
    ImGui::DragFloat("Node Border Width", &style.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
    ImGui::DragFloat("Hovered Node Border Width", &style.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
    ImGui::DragFloat("Selected Node Border Width", &style.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
    ImGui::DragFloat("Pin Rounding", &style.PinRounding, 0.1f, 0.0f, 40.0f);
    ImGui::DragFloat("Pin Border Width", &style.PinBorderWidth, 0.1f, 0.0f, 15.0f);
    ImGui::DragFloat("Link Strength", &style.LinkStrength, 1.0f, 0.0f, 500.0f);
    // ImVec2  SourceDirection;
    // ImVec2  TargetDirection;
    ImGui::DragFloat("Scroll Duration", &style.ScrollDuration, 0.001f, 0.0f, 2.0f);
    ImGui::DragFloat("Flow Marker Distance", &style.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
    ImGui::DragFloat("Flow Speed", &style.FlowSpeed, 1.0f, 1.0f, 2000.0f);
    ImGui::DragFloat("Flow Duration", &style.FlowDuration, 0.001f, 0.0f, 5.0f);
    ImGui::DragFloat2("Pivot Alignment", (float*)&style.PivotAlignment);
    ImGui::DragFloat2("Pivot Size", (float*)&style.PivotSize);
    ImGui::DragFloat2("Pivot Scale", (float*)&style.PivotScale);
    ImGui::DragFloat("Pin Corners", &style.PinCorners);
    ImGui::DragFloat("Pin Radius", &style.PinRadius, 0.001f, 0.0f, 5.0f);
    ImGui::DragFloat("Pin Arrow Size", &style.PinArrowSize, 0.001f, 0.0f, 5.0f);
    ImGui::DragFloat("Pin Arrow Width", &style.PinArrowWidth, 0.001f, 0.0f, 5.0f);
    ImGui::DragFloat("Group Rounding", &style.GroupRounding, 0.1f, 0.0f, 40.0f);
    ImGui::DragFloat("Group Border Width", &style.GroupBorderWidth, 0.1f, 0.0f, 15.0f);
    ImGui::DragFloat("Highlight Connected Links", &style.HighlightConnectedLinks);
    // ImGui::DragFloat("Snap Link To Pin Dir", &style.SnapLinkToPinDir);
}

} // namespace Cool