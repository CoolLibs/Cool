#pragma once

#include <reg/reg.hpp>
#include "Link.h"
#include "Node_Concept.h"
#include "NodesCfg_Concept.h"

namespace Cool::Nodes {

template<Node_Concept Node>
class Graph {
public:
    using NodeId = reg::Id<Node>;
    using LinkId = reg::Id<Link>;

    auto add_node(Node const& node) -> NodeId;
    // auto add_link(Link const& link) -> LinkId;

    // void delete_node(NodeId node_id);
    // void delete_link(LinkId link_id);
    // void delete_link_going_to(PinId pin_id);

    // void remove_all_nodes();

    /**
     * @brief Assumes that pin is an input pin and returns the node that is connected to it (or nullptr if there is none)
     */
    // const Node* find_input_node(const Pin& pin) const;
    // const Node& find_node_with_output_pin(PinId pin_id) const;
    // const Pin&  find_pin(PinId id);
    // bool        has_no_successor(const Node& node) const;

    auto begin() { return _nodes.begin(); }
    auto begin() const { return _nodes.begin(); }
    auto end() { return _nodes.end(); }
    auto end() const { return _nodes.end(); }

private:
    template<NodesCfg_Concept NodesConfig>
    friend class Editor;

    reg::Registry<Node> _nodes;
    reg::Registry<Link> _links;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // try
        // {
        archive(
            cereal::make_nvp("Nodes", _nodes),
            cereal::make_nvp("Links", _links)
        );
        // }
        // catch (const std::exception&)
        // {
        //     _nodes.clear();
        //     _links.clear();
        //     throw;
        // }
    }
};

} // namespace Cool::Nodes

#include "Graph.tpp"