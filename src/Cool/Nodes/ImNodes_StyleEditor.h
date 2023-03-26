#pragma once

#include <imnodes/imnodes.h>
namespace Cool {

class ImNodes_StyleEditor {
public:
    void widget();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        archive(
            cereal::make_nvp("NodeBackground", ImNodes::GetStyle().Colors[ImNodesCol_NodeBackground]),
            cereal::make_nvp("NodeBackgroundHovered", ImNodes::GetStyle().Colors[ImNodesCol_NodeBackgroundHovered]),
            cereal::make_nvp("NodeBackgroundSelected", ImNodes::GetStyle().Colors[ImNodesCol_NodeBackgroundSelected]),
            cereal::make_nvp("NodeOutline", ImNodes::GetStyle().Colors[ImNodesCol_NodeOutline]),
            cereal::make_nvp("TitleBar", ImNodes::GetStyle().Colors[ImNodesCol_TitleBar]),
            cereal::make_nvp("TitleBarHovered", ImNodes::GetStyle().Colors[ImNodesCol_TitleBarHovered]),
            cereal::make_nvp("TitleBarSelected", ImNodes::GetStyle().Colors[ImNodesCol_TitleBarSelected]),
            cereal::make_nvp("Link", ImNodes::GetStyle().Colors[ImNodesCol_Link]),
            cereal::make_nvp("LinkHovered", ImNodes::GetStyle().Colors[ImNodesCol_LinkHovered]),
            cereal::make_nvp("LinkSelected", ImNodes::GetStyle().Colors[ImNodesCol_LinkSelected]),
            cereal::make_nvp("Pin", ImNodes::GetStyle().Colors[ImNodesCol_Pin]),
            cereal::make_nvp("PinHovered", ImNodes::GetStyle().Colors[ImNodesCol_PinHovered]),
            cereal::make_nvp("BoxSelector", ImNodes::GetStyle().Colors[ImNodesCol_BoxSelector]),
            cereal::make_nvp("BoxSelectorOutline", ImNodes::GetStyle().Colors[ImNodesCol_BoxSelectorOutline]),
            cereal::make_nvp("GridBackground", ImNodes::GetStyle().Colors[ImNodesCol_GridBackground]),
            cereal::make_nvp("GridLine", ImNodes::GetStyle().Colors[ImNodesCol_GridLine]),
            cereal::make_nvp("GridLinePrimary", ImNodes::GetStyle().Colors[ImNodesCol_GridLinePrimary]),
            cereal::make_nvp("MiniMapBackground", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapBackground]),
            cereal::make_nvp("MiniMapBackgroundHovered", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapBackgroundHovered]),
            cereal::make_nvp("MiniMapOutline", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapOutline]),
            cereal::make_nvp("MiniMapOutlineHovered", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapOutlineHovered]),
            cereal::make_nvp("MiniMapNodeBackground", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapNodeBackground]),
            cereal::make_nvp("MiniMapNodeBackgroundHovered", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapNodeBackgroundHovered]),
            cereal::make_nvp("MiniMapNodeBackgroundSelected", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapNodeBackgroundSelected]),
            cereal::make_nvp("MiniMapNodeOutline", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapNodeOutline]),
            cereal::make_nvp("MiniMapLink", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapLink]),
            cereal::make_nvp("MiniMapLinkSelected", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapLinkSelected]),
            cereal::make_nvp("MiniMapCanvas", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapCanvas]),
            cereal::make_nvp("MiniMapCanvasOutline", ImNodes::GetStyle().Colors[ImNodesCol_MiniMapCanvasOutline])
        );
        // #else
        //         (void)archive;
        // #endif
    }
};

} // namespace Cool