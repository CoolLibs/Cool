
namespace Cool {

template<Node_Concept Node>
auto Graph<Node>::add_node(Node const& node) -> NodeId
{
    return _nodes.create(node);
}

template<Node_Concept Node>
void Graph<Node>::remove_node(NodeId const& node_id)
{
    const auto node = _nodes.get(node_id);
    if (!node)
        return;

    { // Remove links connected to the node
        std::unique_lock lock{_links.mutex()};
        std::erase_if(_links.underlying_container(), [&](auto const& pair) {
            auto const& link = pair.second;
            return std::any_of(node->output_pins().begin(), node->output_pins().end(), [&](auto const& pin) {
                       return link.from_pin_id == pin.id();
                   })
                   || std::any_of(node->input_pins().begin(), node->input_pins().end(), [&](auto const& pin) {
                          return link.to_pin_id == pin.id();
                      });
        });
    }

    // Remove the node
    _nodes.destroy(node_id);
}

template<Node_Concept Node>
void Graph<Node>::remove_all_nodes()
{
    _nodes.clear();
    _links.clear();
}

template<Node_Concept Node>
auto Graph<Node>::add_link(Link link) -> LinkId
{
    return _links.create(link);
}

template<Node_Concept Node>
void Graph<Node>::remove_link(LinkId const& id)
{
    _links.destroy(id);
}

template<Node_Concept Node>
void Graph<Node>::remove_link_going_into(PinId const& pin_id)
{
    std::unique_lock lock{_links.mutex()};
    std::erase_if(_links.underlying_container(), [&](auto const& pair) {
        return pair.second.to_pin_id == pin_id;
    });
}

template<Node_Concept Node>
auto Graph<Node>::input_node_id(PinId const& pin_id, OutputPin* out__output_pin) const -> NodeId
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

// const Node* Graph::find_input_node(const Pin& pin) const
// {
//     assert(pin.kind() == PinKind::Input);

//     const auto link_it = std::find_if(links.begin(), links.end(), [&](const Link& link) {
//         return link.to_pin_id == pin.id();
//     });

//     // waiting for xcode to support std::ranges::find_if
//     //  const auto link_it = std::ranges::find_if(links, [&](const Link& link) {
//     //      return link.to_pin_id == pin.id();
//     //  });
//     if (link_it == links.end())
//     {
//         return nullptr;
//     }
//     else
//     {
//         return &find_node_with_output_pin(link_it->from_pin_id);
//     }
// }

// const Node& Graph::find_node_with_output_pin(PinId pin_id) const
// {
//     return *std::find_if(nodes.begin(), nodes.end(), [&](const Node& node) {
//         return node.output_pin.id() == pin_id;
//     });

//     // waiting for xcode to support std::ranges::find_if
//     // return *std::ranges::find_if(nodes, [&](const Node& node) {
//     //     return node.output_pin.id() == pin_id;
//     // });
// }

// const Pin& Graph::find_pin(PinId id)
// {
//     for (const auto& node : nodes)
//     {
//         if (node.output_pin.id() == id)
//         {
//             return node.output_pin;
//         }
//         for (const auto& pin : node.input_pins)
//         {
//             if (pin.id() == id)
//             {
//                 return pin;
//             }
//         }
//     }
//     throw std::invalid_argument("pin not found");
// }

// bool Graph::has_no_successor(const Node& node) const
// {
//     return std::find_if(links.begin(), links.end(), [&](const Link& link) {
//                return link.from_pin_id == node.output_pin.id();
//            }) == links.end();

//     // waiting for xcode to support std::ranges::find_if
//     // return std::ranges::find_if(links, [&](const Link& link) {
//     //            return link.from_pin_id == node.output_pin.id();
//     //        }) == links.end();
// }

} // namespace Cool