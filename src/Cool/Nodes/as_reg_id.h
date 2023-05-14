#pragma once
#include "Graph.h"
#include "as_ed_id.h"
#include "ed.h"

namespace Cool {

inline auto as_reg_id(ed::NodeId const& ed_id, Graph const& graph) -> NodeId
{
    for (auto const& [reg_id, _] : graph.nodes())
    {
        if (ed::NodeId{as_ed_id(reg_id)} == ed_id)
            return reg_id;
    }
    return {};
}

inline auto as_reg_id(ed::LinkId const& ed_id, Graph const& graph) -> LinkId
{
    for (auto const& [reg_id, _] : graph.links())
    {
        if (ed::LinkId{as_ed_id(reg_id)} == ed_id)
            return reg_id;
    }
    return {};
}

} // namespace Cool