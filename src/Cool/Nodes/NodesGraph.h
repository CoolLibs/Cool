#pragma once

#include <reg/reg.hpp>
#include "Link.h"
#include "LinkId.h"
#include "Node.h"
#include "NodeId.h"

namespace Cool {

class NodesGraph {
public:
    auto add_node(Node) -> NodeId;
    void remove_node(NodeId const&);
    void remove_all_nodes();

    auto add_link(Link const&) -> LinkId;
    /// No checks to remove existing links that would be incompatible with this one.
    auto add_link_unchecked(Link const&) -> LinkId;
    void remove_link(LinkId const&);
    void remove_link_going_into(PinId const&);
    void remove_link_coming_from(PinId const&);

    auto find_node_connected_to_input_pin(PinId const&, OutputPin* out__output_pin = nullptr) const -> NodeId;
    auto find_node_connected_to_output_pin(PinId const&) const -> NodeId;
    auto find_node_containing_pin(PinId const&) const -> NodeId;

    auto nodes() -> auto& { return _nodes; }
    auto nodes() const -> auto const& { return _nodes; }
    auto links() -> auto& { return _links; }
    auto links() const -> auto const& { return _links; }

    template<Node_Concept NodeT>
    [[nodiscard]] auto try_get_node(NodeId const& id) -> NodeT*
    {
        Node* const node = _nodes.get_mutable_ref(id);
        if (!node)
            return nullptr;
        return &node->downcast<NodeT>();
    }
    template<Node_Concept NodeT>
    [[nodiscard]] auto try_get_node(NodeId const& id) const -> NodeT const*
    {
        Node const* const node = _nodes.get_ref(id);
        if (!node)
            return nullptr;
        return &node->downcast<NodeT>();
    }
    template<Node_Concept NodeT>
    void for_each_node(std::function<void(NodeT&)> const& callback)
    {
        std::unique_lock lock{nodes().mutex()};
        for (auto& [_, node] : nodes())
        {
            callback(node.template downcast<NodeT>());
        }
    }
    template<Node_Concept NodeT>
    void for_each_node(std::function<void(NodeT const&)> const& callback) const
    {
        std::shared_lock lock{nodes().mutex()};
        for (auto const& [_, node] : nodes())
        {
            callback(node.template downcast<NodeT>());
        }
    }
    void for_each_link_connected_to_node(Node const&, std::function<void(Link const&)> const& callback) const;

private:
    reg::RawRegistry<Node> _nodes;
    reg::RawRegistry<Link> _links;

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