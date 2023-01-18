#pragma once
#include "Cool/Nodes/NodesLibrary.h"
#include "NodeDefinition_Concept.h"
#include "Node_Concept.h"

namespace Cool {

template<typename Cfg>
concept NodesCfg_Concept =
    Node_Concept<typename Cfg::NodeT>
    && NodeDefinition_Concept<typename Cfg::NodeDefinitionT>
    && requires(Cfg cfg, typename Cfg::NodeT node, Cool::NodeId id, Cool::NodeCategoryIdentifier<typename Cfg::NodeDefinitionT> cat_id) {
           // clang-format off
            { cfg.make_node(cat_id) } -> std::convertible_to<typename Cfg::NodeT>;
            { cfg.name(node) } -> std::convertible_to<std::string>;
            cfg.imgui_node_body(node, id);
           // clang-format on
       };

} // namespace Cool