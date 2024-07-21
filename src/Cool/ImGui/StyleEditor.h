#pragma once
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "imgui-node-editor/imgui_node_editor.h"

namespace ser20 {
template<class Archive>
void serialize(Archive& archive, ax::NodeEditor::Style& style)
{
    archive(
        ser20::make_nvp("NodePadding", style.NodePadding),
        ser20::make_nvp("NodeRounding", style.NodeRounding),
        ser20::make_nvp("NodeBorderWidth", style.NodeBorderWidth),
        ser20::make_nvp("HoveredNodeBorderWidth", style.HoveredNodeBorderWidth),
        ser20::make_nvp("SelectedNodeBorderWidth", style.SelectedNodeBorderWidth),
        ser20::make_nvp("PinRounding", style.PinRounding),
        ser20::make_nvp("PinBorderWidth", style.PinBorderWidth),
        ser20::make_nvp("LinkStrength", style.LinkStrength),
        ser20::make_nvp("SourceDirection", style.SourceDirection),
        ser20::make_nvp("TargetDirection", style.TargetDirection),
        ser20::make_nvp("ScrollDuration", style.ScrollDuration),
        ser20::make_nvp("FlowMarkerDistance", style.FlowMarkerDistance),
        ser20::make_nvp("FlowSpeed", style.FlowSpeed),
        ser20::make_nvp("FlowDuration", style.FlowDuration),
        ser20::make_nvp("PivotAlignment", style.PivotAlignment),
        ser20::make_nvp("PivotSize", style.PivotSize),
        ser20::make_nvp("PivotScale", style.PivotScale),
        ser20::make_nvp("PinCorners", style.PinCorners),
        ser20::make_nvp("PinRadius", style.PinRadius),
        ser20::make_nvp("PinArrowSize", style.PinArrowSize),
        ser20::make_nvp("PinArrowWidth", style.PinArrowWidth),
        ser20::make_nvp("GroupRounding", style.GroupRounding),
        ser20::make_nvp("GroupBorderWidth", style.GroupBorderWidth),
        ser20::make_nvp("HighlightConnectedLinks", style.HighlightConnectedLinks),
        ser20::make_nvp("SnapLinkToPinDir", style.SnapLinkToPinDir)
    );
}
} // namespace ser20

namespace Cool {

class StyleEditor {
public:
    StyleEditor();
    ~StyleEditor();

    void imgui();

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("ImGui Style", ImGui::GetStyle()),
            ser20::make_nvp("ImGuiExtras Style", ImGuiExtras::GetStyle()),
            ser20::make_nvp("ImNodes Style", ax::NodeEditor::GetStyle())
        );
    }
};

} // namespace Cool