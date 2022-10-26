#pragma once

#include <reg/reg.hpp>
#include "Link.h"
#include "LinkId.h"
#include "NodeId.h"
#include "Node_Concept.h"
#include "NodesCfg_Concept.h"

namespace Cool {

template<Node_Concept Node>
class Graph {
public:
    auto add_node(Node const&) -> NodeId;
    void remove_node(NodeId const&);
    void remove_all_nodes();

    // auto add_link(Link const& link) -> LinkId;
    // void delete_link(LinkId link_id);
    // void delete_link_going_to(PinId pin_id);

    /**
     * @brief Assumes that pin is an input pin and returns the node that is connected to it (or nullptr if there is none)
     */
    // const Node* find_input_node(const Pin& pin) const;
    // const Node& find_node_with_output_pin(PinId pin_id) const;
    // const Pin&  find_pin(PinId id);
    // bool        has_no_successor(const Node& node) const;

    auto nodes() -> auto& { return _nodes; }
    auto nodes() const -> auto const& { return _nodes; }

private:
    reg::Registry<Node> _nodes;
    reg::Registry<Link> _links;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Nodes", _nodes),
            cereal::make_nvp("Links", _links)
        );
    }
};

} // namespace Cool

#include "Graph.tpp"