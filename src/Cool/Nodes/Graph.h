#pragma once

#include <reg/reg.hpp>
#include "BaseNode.h"
#include "Cool/Nodes/BaseNode.h"
#include "Link.h"
#include "LinkId.h"
#include "NodeId.h"

namespace Cool {

using NodeOwner = std::shared_ptr<BaseNode>;

class GraphImpl {
public:
    auto add_node(NodeOwner) -> NodeId;
    void remove_node(NodeId const&);
    void remove_all_nodes();

    auto add_link(Link) -> LinkId;
    void remove_link(LinkId const&);
    void remove_link_going_into(PinId const&);
    void remove_link_coming_from(PinId const&);

    auto input_node_id(PinId const&, OutputPin* out__output_pin = nullptr) const -> NodeId;

    auto nodes() -> auto& { return _nodes; }
    auto nodes() const -> auto const& { return _nodes; }
    auto links() -> auto& { return _links; }
    auto links() const -> auto const& { return _links; }

protected:
    [[nodiscard]] auto try_get_node_impl(NodeId const& id) -> BaseNode*;
    [[nodiscard]] auto try_get_node_impl(NodeId const& id) const -> BaseNode const*;
    void               for_each_node_impl(std::function<void(BaseNode&)> const& callback);
    void               for_each_node_impl(std::function<void(BaseNode const&)> const& callback) const;

private:
    reg::RawRegistry<NodeOwner> _nodes;
    reg::RawRegistry<Link>      _links;

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

/// Adds functions that manipulate the concrete node type instead of `Cool::BaseNode`.
template<typename ConcreteNode>
class Graph : public GraphImpl {
public:
    /// Returns a pointer to the node referenced by the `id`, or nullptr if the node does not exist.
    [[nodiscard]] auto try_get_node(NodeId const& id) -> ConcreteNode*
    {
        return static_cast<ConcreteNode*>(try_get_node_impl(id));
    }
    /// Returns a pointer to the node referenced by the `id`, or nullptr if the node does not exist.
    [[nodiscard]] auto try_get_node(NodeId const& id) const -> ConcreteNode const*
    {
        return static_cast<ConcreteNode const*>(try_get_node_impl(id));
    }

    void for_each_node(std::function<void(ConcreteNode&)> const& callback)
    {
        for_each_node_impl([&](BaseNode& node) {
            callback(static_cast<ConcreteNode&>(node));
        });
    }
    void for_each_node(std::function<void(ConcreteNode const&)> const& callback) const
    {
        for_each_node_impl([&](BaseNode const& node) {
            callback(static_cast<ConcreteNode const&>(node));
        });
    }
};

} // namespace Cool