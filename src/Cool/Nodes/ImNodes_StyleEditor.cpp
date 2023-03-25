#include "ImNodes_StyleEditor.h"
#include <imgui.h>
#include <imnodes/imnodes.h>

namespace Cool {

void ImNodes_StyleEditor::widget()
{
    auto const edit_color = [](const char* name, int id) {
        ImColor col{ImNodes::GetStyle().Colors[id]};
        if (ImGui::ColorEdit4(name, (float*)&col.Value))
            ImNodes::GetStyle().Colors[id] = col;
    };
    edit_color("NodeBackground", ImNodesCol_NodeBackground);
    edit_color("NodeBackgroundHovered", ImNodesCol_NodeBackgroundHovered);
    edit_color("NodeBackgroundSelected", ImNodesCol_NodeBackgroundSelected);
    edit_color("NodeOutline", ImNodesCol_NodeOutline);
    edit_color("TitleBar", ImNodesCol_TitleBar);
    edit_color("TitleBarHovered", ImNodesCol_TitleBarHovered);
    edit_color("TitleBarSelected", ImNodesCol_TitleBarSelected);
    edit_color("Link", ImNodesCol_Link);
    edit_color("LinkHovered", ImNodesCol_LinkHovered);
    edit_color("LinkSelected", ImNodesCol_LinkSelected);
    edit_color("Pin", ImNodesCol_Pin);
    edit_color("PinHovered", ImNodesCol_PinHovered);
    edit_color("BoxSelector", ImNodesCol_BoxSelector);
    edit_color("BoxSelectorOutline", ImNodesCol_BoxSelectorOutline);
    edit_color("GridBackground", ImNodesCol_GridBackground);
    edit_color("GridLine", ImNodesCol_GridLine);
    edit_color("GridLinePrimary", ImNodesCol_GridLinePrimary);
    edit_color("MiniMapBackground", ImNodesCol_MiniMapBackground);
    edit_color("MiniMapBackgroundHovered", ImNodesCol_MiniMapBackgroundHovered);
    edit_color("MiniMapOutline", ImNodesCol_MiniMapOutline);
    edit_color("MiniMapOutlineHovered", ImNodesCol_MiniMapOutlineHovered);
    edit_color("MiniMapNodeBackground", ImNodesCol_MiniMapNodeBackground);
    edit_color("MiniMapNodeBackgroundHovered", ImNodesCol_MiniMapNodeBackgroundHovered);
    edit_color("MiniMapNodeBackgroundSelected", ImNodesCol_MiniMapNodeBackgroundSelected);
    edit_color("MiniMapNodeOutline", ImNodesCol_MiniMapNodeOutline);
    edit_color("MiniMapLink", ImNodesCol_MiniMapLink);
    edit_color("MiniMapLinkSelected", ImNodesCol_MiniMapLinkSelected);
    edit_color("MiniMapCanvas", ImNodesCol_MiniMapCanvas);
    edit_color("MiniMapCanvasOutline", ImNodesCol_MiniMapCanvasOutline);
}

} // namespace Cool