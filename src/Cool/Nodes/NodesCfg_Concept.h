#pragma once
#include "NodeDefinition_Concept.h"
#include "Node_Concept.h"

namespace Cool {

template<typename Cfg>
concept NodesCfg_Concept =
    Node_Concept<typename Cfg::NodeT>
    && NodeDefinition_Concept<typename Cfg::NodeDefinitionT>
    && requires(Cfg cfg, typename Cfg::NodeDefinitionT def, typename Cfg::NodeT node) {
           // clang-format off
            { cfg.make_node(def) } -> std::convertible_to<typename Cfg::NodeT>;
            { cfg.name(node) } -> std::convertible_to<std::string>;
           // clang-format on
       };

} // namespace Cool