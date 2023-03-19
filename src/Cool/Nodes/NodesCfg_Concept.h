#pragma once
#include "Cool/Nodes/Graph.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "NodeDefinition_Concept.h"

namespace Cool {

template<typename Cfg>
concept NodesCfg_Concept = true;
// NodeDefinition_Concept<typename Cfg::NodeDefinitionT>
// && requires(Cfg cfg, typename Cfg::NodeT node, Cool::NodeId id, Cool::NodeDefinitionAndCategoryName<typename Cfg::NodeDefinitionT> def_and_cat_name, typename Cfg::NodeDefinitionT def, Graph graph) {
//        // clang-format off
//         { cfg.make_node(def_and_cat_name) } -> std::convertible_to<typename Cfg::NodeT>;
//         cfg.update_node_with_new_definition(node, def, graph);
//         { cfg.name(node) } -> std::convertible_to<std::string>;
//         { cfg.category_name(node) } -> std::convertible_to<std::string>;
//         cfg.imgui_node_body(node, id);
//         cfg.widget_to_rename_node(node);
//         // clang-format on
//    };

} // namespace Cool