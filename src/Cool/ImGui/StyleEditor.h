#pragma once
#include "Cool/ImGui/ImGuiExtrasStyle.h"
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

class StyleEditor {
public:
    StyleEditor();
    ~StyleEditor();

    void imgui();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("ImGui Style", ImGui::GetStyle()),
            cereal::make_nvp("ImGuiExtras Style", ImGuiExtras::GetStyle()),
            cereal::make_nvp("ImNodes Style", ax::NodeEditor::GetStyle())
        );
    }
};

} // namespace Cool