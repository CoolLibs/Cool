#pragma once

#include <reg/reg.hpp>
#include "Link.h"
#include "LinkId.h"
#include "NodeId.h"
#include "Node_Concept.h"

namespace Cool {

template<Node_Concept Node>
class Graph {
public:
    auto add_node(Node const&) -> NodeId;
    void remove_node(NodeId const&);
    void remove_all_nodes();

    auto add_link(Link) -> LinkId;
    void remove_link(LinkId const&);
    void remove_link_going_into(PinId const&);
    void remove_link_coming_from(PinId const&);

    /**
     * @brief Assumes that pin is an input pin and returns the node that is connected to it (or nullptr if there is none)
     */
    // const Node* find_input_node(const Pin& pin) const;
    // auto find_node_with_output_pin(PinId const&) const -> Node const*;
    // const Pin&  find_pin(PinId id);
    // bool        has_no_successor(const Node& node) const;
    auto input_node_id(PinId const&, OutputPin* out__output_pin = nullptr) const -> NodeId;

    auto nodes() -> auto& { return _nodes; }
    auto nodes() const -> auto const& { return _nodes; }
    auto links() -> auto& { return _links; }
    auto links() const -> auto const& { return _links; }

private:
    reg::RawRegistry<Node> _nodes;
    reg::RawRegistry<Link> _links;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(
            cereal::make_nvp("Nodes", _nodes),
            cereal::make_nvp("Links", _links)
        );
#else
        (void)archive;
#endif
    }
};

} // namespace Cool

#include "Graph.tpp"