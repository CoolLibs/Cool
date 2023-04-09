#include "StyleEditor.h"
#include <cereal/archives/json.hpp>
#include "Cool/Path/Path.h"
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
    auto is = std::ifstream{Cool::Path::root() / "style.json"};
    if (!is.is_open())
        return;
    try
    {
        auto archive = cereal::JSONInputArchive{is};
        archive(
            ImGui::GetStyle(),
            ax::NodeEditor::GetStyle()
        );
    }
    catch (...)
    {
    }
}

StyleEditor::~StyleEditor()
{
    auto os = std::ofstream{Cool::Path::root() / "style.json"};
    {
        auto archive = cereal::JSONOutputArchive{os};
        archive(
            cereal::make_nvp("ImGui", ImGui::GetStyle()),
            cereal::make_nvp("ImNodes", ax::NodeEditor::GetStyle())
        );
    }
}

} // namespace Cool