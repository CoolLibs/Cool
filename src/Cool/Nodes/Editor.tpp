
#include <Cool/Log/ToUser.h>
// #include <GLFW/glfw3.h>
#include <imnodes/imnodes_internal.h>

namespace Cool::Nodes {

// bool Editor::tree_has_changed()
// {
//     bool b             = _graph_has_changed;
//     _graph_has_changed = false;
//     return b;
// }

// void Editor::on_graph_change()
// {
//     _graph_has_changed = true;
// }

// bool Editor::wants_to_delete_selection() const
// {
//     return _window_is_hovered &&
//            !ImGui::GetIO().WantTextInput &&
//            (ImGui::IsKeyReleased(ImGuiKey_Delete) ||
//             ImGui::IsKeyReleased(ImGuiKey_Backspace) ||
//             ImGui::IsKeyReleased(ImGuiKey_X) /*TODO(JF) This should be a logical key, not a physical one*/);
// }

// static void show_node_pins(const Node& node)
// {
//     ImGui::BeginGroup();
//     for (const auto& pin : node.input_pins())
//     {
//         pin.show();
//     }
//     ImGui::EndGroup();
//     ImGui::SameLine(200.f); // TODO(JF) Don't use a hardcoded value
//     for (const auto& pin : node.output_pins())
//     {
//         pin.show();
//     }
// }

// static void show_node_params(Node& node, std::function<void()> /* on_change */)
// {
//     ImGui::BeginGroup();
//     ImGui::PushID(&node);
//     ImGui::PushItemWidth(200.f); // TODO(JF) Don't use a hardcoded value
//     // node.parameter_list.imgui(on_change); // TODO(JF)
//     ImGui::PopItemWidth();
//     ImGui::PopID();
//     ImGui::EndGroup();
// }

template<NodesCfg_Concept NodesCfg>
void draw_node(typename NodesCfg::NodeT& node)
{
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(NodesCfg::name(node));
    ImNodes::EndNodeTitleBar();
    // show_node_pins(node);
    // show_node_params(node);
}

// static void show_link(const Link& link)
// {
//     ImNodes::Link(link.id, link.from_pin_id, link.to_pin_id);
// }

// bool Editor::handle_link_creation()
// {
//     int from_pin_id, to_pin_id;
//     if (ImNodes::IsLinkCreated(&from_pin_id, &to_pin_id))
//     {
//         _graph.delete_link_going_to(PinId{to_pin_id});
//         _graph.add_link(Link{
//             .from_pin_id = PinId{from_pin_id},
//             .to_pin_id   = PinId{to_pin_id},
//         });
//         return true;
//     }
//     return false;
// }

// bool Editor::handle_link_deletion()
// {
//     bool has_deleted_some = false;
//     {
//         int link_id;
//         if (ImNodes::IsLinkDestroyed(&link_id))
//         {
//             _graph.delete_link(LinkId{link_id});
//             has_deleted_some = true;
//         }
//     }

//     {
//         const int num_selected = ImNodes::NumSelectedLinks();
//         if (num_selected > 0 && wants_to_delete_selection())
//         {
//             has_deleted_some = true;
//             static std::vector<int> selected_links;
//             selected_links.resize(static_cast<size_t>(num_selected));
//             ImNodes::GetSelectedLinks(selected_links.data());
//             for (const int link_id : selected_links)
//             {
//                 _graph.delete_link(LinkId{link_id});
//             }
//         }
//     }
//     return has_deleted_some;
// }

// bool Editor::handle_node_deletion()
// {
//     const int num_selected = ImNodes::NumSelectedNodes();
//     if (num_selected > 0 && wants_to_delete_selection())
//     {
//         static std::vector<int> selected_nodes;
//         selected_nodes.resize(static_cast<size_t>(num_selected));
//         ImNodes::GetSelectedNodes(selected_nodes.data());
//         for (const int node_id : selected_nodes)
//         {
//             _graph.delete_node(NodeId{node_id});
//         }
//         ImNodes::ClearNodeSelection();
//         return true;
//     }
//     return false;
// }

template<NodesCfg_Concept NodesCfg>
auto Editor<NodesCfg>::wants_to_open_nodes_menu() -> bool
{
    return (/* _window_is_hovered && */ (ImGui::IsMouseReleased(ImGuiMouseButton_Right) || ImGui::IsKeyReleased(ImGuiKey_A)));
}

template<NodesCfg_Concept NodesCfg>
void Editor<NodesCfg>::open_nodes_menu()
{
    ImGui::OpenPopup("_nodes_library");
    // _factory.clear_filter();
    _next_node_position = ImGui::GetMousePosOnOpeningCurrentPopup();
}

template<NodesCfg_Concept NodesCfg>
void Editor<NodesCfg>::show_nodes_library_menu_ifn(SetDirty_Ref set_dirty)
{
    if (wants_to_open_nodes_menu())
    {
        open_nodes_menu();
    }
    if (ImGui::BeginPopup("_nodes_library"))
    {
        if (imgui_nodes_menu())
        {
            ImGui::CloseCurrentPopup();
            set_dirty(_graph_dirty_flag);
        }
        ImGui::EndPopup();
    }
}

template<NodesCfg_Concept NodesCfg>
void Editor<NodesCfg>::imgui_window(SetDirty_Ref set_dirty)
{
    ImNodes::SetCurrentContext(&*_context);
    // bool node_graph_has_changed = false;
    ImGui::Begin("Nodes");
    // _window_is_hovered = ImGui::IsWindowHovered( // TODO(JF) Do we need to compute it here? Can't we do it after ImNodes::BeginNodeEditor()?
    //     ImGuiHoveredFlags_ChildWindows |
    //     ImGuiHoveredFlags_NoPopupHierarchy
    // );
    ImNodes::BeginNodeEditor();
    {
        show_nodes_library_menu_ifn(set_dirty);
        for (auto& [id, node] : _graph._nodes)
        {
            ImNodes::BeginNode(id);
            draw_node<NodesCfg>(node);
            ImNodes::EndNode();
        }
        // for (const auto& link : _graph.links)
        // {
        //     show_link(link);
        // }
    }
    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();
    // node_graph_has_changed |= handle_link_creation();
    // node_graph_has_changed |= handle_link_deletion();
    // node_graph_has_changed |= handle_node_deletion();
    ImGui::End();
    // if (node_graph_has_changed)
    // {
    //     on_graph_change();
    // }
}

template<NodesCfg_Concept NodesCfg>
auto Editor<NodesCfg>::imgui_nodes_menu() -> bool
{
    // const std::optional<Node> node = _factory.imgui();
    // if (node.has_value())
    // {
    //     _graph.add_node(*node);
    //     ImNodes::SetNodeScreenSpacePos(node->id, _next_node_position);
    //     return true;
    // }
    // else
    {
        return false;
    }
}

// void Editor::update_templates_and_nodes()
// {
//     _all_nodes_have_a_valid_template = true;
//     _factory.reload_templates();
//     for (auto& node : _graph.nodes)
//     {
//         const auto node_template = std::find_if(
//             _factory.templates().begin(),
//             _factory.templates().end(),
//             [&](const NodeTemplate& node_template) {
//                 return node_template.name == node.node_template_name;
//             }
//         );

//         // wainting for xcode to support std::ranges::find_if
//         // const auto node_template = std::ranges::find_if(_factory.templates(), [&](const NodeTemplate& node_template) {
//         // return node_template.name == node.node_template_name;
//         // });
//         if (node_template == _factory.templates().end())
//         {
//             _all_nodes_have_a_valid_template = false;
//             Cool::Log::ToUser::warning(
//                 "is0 " + node.node_template_name,
//                 fmt::format("Can't find node file '{0}.is0' Your graph can't be compiled.\nEither add a '{0}.is0' file or delete all {0} nodes.", node.node_template_name)
//             );
//         }
//         else
//         {
//             // Update params
//             node.parameter_list = Cool::ParameterU::update_parameters(node_template->parameters, node.parameter_list);
//             // Update input pins
//             const auto nb_pins = node_template->sdf_identifiers.size();
//             if (nb_pins < node.input_pins.size())
//             {
//                 for (size_t i = nb_pins; i < node.input_pins.size(); ++i)
//                 {
//                     _graph.delete_link_going_to(node.input_pins[i].id());
//                 }
//             }
//             node.input_pins.resize(nb_pins);
//         }
//     }
//     on_graph_change();
// }

} // namespace Cool::Nodes