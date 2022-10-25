
namespace Cool {

template<Node_Concept Node>
auto Graph<Node>::add_node(const Node& node) -> NodeId
{
    return _nodes.create(node);
}

// void Graph::remove_all_nodes()
// {
//     nodes.clear();
//     links.clear();
// }

// void Graph::add_link(Link link)
// {
//     links.push_back(link);
// }

// void Graph::delete_node(NodeId node_id)
// {
//     const auto node = std::find_if(nodes.begin(), nodes.end(), [&](const Node& node) {
//         return node.id == node_id;
//     });

//     std::erase_if(links, [&](const Link& link) {
//         return link.from_pin_id == node->output_pin.id() ||
//                std::any_of(node->input_pins.begin(), node->input_pins.end(), [&](const auto& pin) {
//                    return link.to_pin_id == pin.id();
//                });
//     });

//     // waiting for xcode to support std::ranges::find_if && std::ranges::any_of
//     //  const auto node = std::ranges::find_if(nodes, [&](const Node& node) {
//     //      return node.id == node_id;
//     //  });
//     // std::erase_if(links, [&](const Link& link) {
//     //     return link.from_pin_id == node->output_pin.id() ||
//     //            std::ranges::any_of(node->input_pins, [&](const auto& pin) {
//     //                return link.to_pin_id == pin.id();
//     //            });
//     // });
//     nodes.erase(node);
// }

// void Graph::delete_link(LinkId link_id)
// {
//     std::erase_if(links, [&](const Link& link) {
//         return link.id == link_id;
//     });
// }

// void Graph::delete_link_going_to(PinId pin_id)
// {
//     std::erase_if(links, [&](const Link& link) {
//         return link.to_pin_id == pin_id;
//     });
// }

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