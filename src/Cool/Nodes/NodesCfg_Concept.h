#pragma once
#include "NodeDefinition_Concept.h"
#include "Node_Concept.h"

namespace Cool {

template<typename Cfg>
concept NodesCfg_Concept =
    Node_Concept<typename Cfg::NodeT>
    && NodeDefinition_Concept<typename Cfg::NodeDefinitionT>
    && requires(typename Cfg::NodeDefinitionT def, typename Cfg::NodeT node) {
           // clang-format off
            { Cfg::make_node(def) } -> std::convertible_to<typename Cfg::NodeT>;
            { Cfg::name(node) } -> std::convertible_to<std::string>;
           // clang-format on
       };

} // namespace Cool