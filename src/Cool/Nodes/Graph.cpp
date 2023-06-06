#include "Graph.h"

namespace Cool {

auto Graph::add_node(Node node) -> NodeId
{
    return _nodes.create_raw(std::move(node));
}

void Graph::remove_node(NodeId const& node_id)
{
    auto const maybe_node = _nodes.get(node_id);
    if (!maybe_node)
        return;

    auto const& node = *maybe_node;

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

void Graph::remove_all_nodes()
{
    _nodes.clear();
    _links.clear();
}

auto Graph::add_link(Link link) -> LinkId
{
    remove_link_going_into(link.to_pin_id);
    return _links.create_raw(link);
}

void Graph::remove_link(LinkId const& id)
{
    _links.destroy(id);
}

void Graph::remove_link_going_into(PinId const& pin_id)
{
    std::unique_lock lock{_links.mutex()};
    std::erase_if(_links.underlying_container(), [&](auto const& pair) {
        return pair.second.to_pin_id == pin_id;
    });
}

void Graph::remove_link_coming_from(PinId const& pin_id)
{
    std::unique_lock lock{_links.mutex()};
    std::erase_if(_links.underlying_container(), [&](auto const& pair) {
        return pair.second.from_pin_id == pin_id;
    });
}

auto Graph::find_node_connected_to_input_pin(PinId const& pin_id, OutputPin* out__output_pin) const -> NodeId
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
            for (OutputPin const& output_pin : other_node.output_pins())
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

auto Graph::find_node_connected_to_output_pin(PinId const& pin_id) const -> NodeId
{
    std::shared_lock nodes_lock{nodes().mutex()};
    std::shared_lock links_lock{links().mutex()};

    for (auto const& pair : links())
    {
        auto const& link = pair.second;
        // Check that `link` is connected to `pin_id`
        if (link.from_pin_id != pin_id)
            continue;

        // Find the other node connected to that link
        for (auto const& [other_node_id, other_node] : nodes())
        {
            for (InputPin const& input_pin : other_node.input_pins())
            {
                if (input_pin.id() == link.to_pin_id)
                {
                    return other_node_id;
                }
            }
        }
    }

    return {};
}

auto Graph::find_node_containing_pin(PinId const& pin_id) const -> NodeId
{
    for (auto const& [node_id, node] : nodes())
    {
        for (auto const& pin : node.input_pins())
        {
            if (pin.id() == pin_id)
                return node_id;
        }
        for (auto const& pin : node.output_pins())
        {
            if (pin.id() == pin_id)
                return node_id;
        }
    }
    return {};
}

} // namespace Cool