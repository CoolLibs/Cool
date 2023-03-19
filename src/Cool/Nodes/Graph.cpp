#include "Graph.h"
#include <cstddef>

namespace Cool {

auto GraphImpl::add_node(NodeOwner node) -> NodeId
{
    return _nodes.create_raw(std::move(node));
}

void GraphImpl::remove_node(NodeId const& node_id)
{
    auto const maybe_node = _nodes.get(node_id);
    if (!maybe_node)
        return;

    auto const& node = **maybe_node;

    { // Remove links connected to the node
        std::unique_lock lock{_links.mutex()};
        std::erase_if(_links.underlying_container(), [&](auto const& pair) {
            auto const& link = pair.second;
            return std::any_of(node.output_pins().begin(), node.output_pins().end(), [&](auto const& pin) {
                       return link.from_pin_id == pin.id();
                   })
                   || std::any_of(node.input_pins().begin(), node.input_pins().end(), [&](auto const& pin) {
                          return link.to_pin_id == pin.id();
                      });
        });
    }

    // Remove the node
    _nodes.destroy(node_id);
}

void GraphImpl::remove_all_nodes()
{
    _nodes.clear();
    _links.clear();
}

auto GraphImpl::add_link(Link link) -> LinkId
{
    return _links.create_raw(link);
}

void GraphImpl::remove_link(LinkId const& id)
{
    _links.destroy(id);
}

void GraphImpl::remove_link_going_into(PinId const& pin_id)
{
    std::unique_lock lock{_links.mutex()};
    std::erase_if(_links.underlying_container(), [&](auto const& pair) {
        return pair.second.to_pin_id == pin_id;
    });
}

void GraphImpl::remove_link_coming_from(PinId const& pin_id)
{
    std::unique_lock lock{_links.mutex()};
    std::erase_if(_links.underlying_container(), [&](auto const& pair) {
        return pair.second.from_pin_id == pin_id;
    });
}

auto GraphImpl::input_node_id(PinId const& pin_id, OutputPin* out__output_pin) const -> NodeId
{
    std::shared_lock nodes_lock{nodes().mutex()};
    std::shared_lock links_lock{links().mutex()};

    for (auto const& pair : links())
    {
        auto const& link = pair.second;
        // Check that `link` is connected to `pin_id`
        if (link.to_pin_id != pin_id)
            continue;

        // Find the other node connected to that link
        for (auto const& [other_node_id, other_node] : nodes())
        {
            for (OutputPin const& output_pin : other_node->output_pins())
            {
                if (output_pin.id() == link.from_pin_id)
                {
                    if (out__output_pin)
                        *out__output_pin = output_pin;
                    return other_node_id;
                }
            }
        }
    }

    return {};
}

auto GraphImpl::try_get_node_impl(NodeId const& id) -> BaseNode*
{
    NodeOwner const* const node = _nodes.get_ref(id);
    if (!node)
        return nullptr;
    return node->get();
}
auto GraphImpl::try_get_node_impl(NodeId const& id) const -> BaseNode const*
{
    NodeOwner const* const node = _nodes.get_ref(id);
    if (!node)
        return nullptr;
    return node->get();
}

void GraphImpl::for_each_node_impl(std::function<void(BaseNode&)> const& callback)
{
    std::unique_lock lock{nodes().mutex()};
    for (auto& [_, node] : nodes())
    {
        callback(*node);
    }
}
void GraphImpl::for_each_node_impl(std::function<void(BaseNode const&)> const& callback) const
{
    std::shared_lock lock{nodes().mutex()};
    for (auto const& [_, node] : nodes())
    {
        callback(*node);
    }
}

} // namespace Cool