
#include <Cool/Log/ToUser.h>
// #include <GLFW/glfw3.h>
#include <imnodes/imnodes_internal.h>
#include "Cool/Nodes/NodeDefinition_Concept.h"
#include "ImNodesHelpers.h"
#include "NodesLibrary.h"
#include "imgui.h"
#include "imnodes/imnodes.h"

namespace Cool {

//
// template<NodesCfg_Concept NodesCfg>
// bool NodesEditor<NodesCfg>::tree_has_changed()
// {
//     bool b             = _graph_has_changed;
//     _graph_has_changed = false;
//     return b;
// }

//
// template<NodesCfg_Concept NodesCfg>
// void NodesEditor<NodesCfg>::on_graph_change()
// {
//     _graph_has_changed = true;
// }

void draw_node_pins(Node_Concept auto const& node)
{
    ImGui::BeginGroup();
    for (auto const& pin : node.input_pins())
        pin.show();
    ImGui::EndGroup();

    ImGui::SameLine(200.f); // TODO(JF) Don't use a hardcoded value

    ImGui::BeginGroup();
    for (auto const& pin : node.output_pins())
        pin.show();
    ImGui::EndGroup();
}

template<NodesCfg_Concept NodesCfg>
void draw_node_body(typename NodesCfg::NodeT& node, NodeId const& id, NodesCfg const& nodes_cfg)
{
    ImGui::BeginGroup();
    ImGui::PushID(&node);
    ImGui::PushItemWidth(200.f); // TODO(JF) Don't use a hardcoded value

    nodes_cfg.imgui_node_body(node, id);

    ImGui::PopItemWidth();
    ImGui::PopID();
    ImGui::EndGroup();
}

template<NodeDefinition_Concept NodeDefinition>
static auto calc_max_text_width(std::vector<NodeDefinition> const& defs) -> float
{
    float max = 0.f;
    for (auto const& def : defs)
        max = std::max(max, ImGui::CalcTextSize(def.name().c_str()).x);
    return max + 20.f;
}

template<NodesCfg_Concept NodesCfg>
static auto dropdown_to_switch_between_nodes_of_the_same_category(typename NodesCfg::NodeT& node, NodesCfg const& nodes_cfg, NodesLibrary<typename NodesCfg::NodeDefinitionT> const& library, Graph<typename NodesCfg::NodeT>& graph) -> bool
{
    auto const* category = library.get_category(node.category_name());
    if (!category)
        return false;

    bool graph_has_changed = false;

    ImGui::SetNextItemWidth(calc_max_text_width(category->definitions()));
    if (ImGui::BeginCombo(category->name().c_str(), node.definition_name().c_str()))
    {
        for (auto const& def : category->definitions())
        {
            ImGui::PushID(&def);
            bool const is_selected = def.name() == node.definition_name();
            if (ImGui::Selectable(def.name().c_str(), is_selected))
            {
                nodes_cfg.update_node_with_new_definition(node, def, graph);
                graph_has_changed = true;
            }
            ImGui::PopID();
        }
        ImGui::EndCombo();
    }

    return graph_has_changed;
}

template<NodesCfg_Concept NodesCfg>
auto draw_node(typename NodesCfg::NodeT& node, NodeId const& id, NodesCfg const& nodes_cfg, NodesLibrary<typename NodesCfg::NodeDefinitionT> const& library, Graph<typename NodesCfg::NodeT>& graph) -> bool
{
    ImNodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(nodes_cfg.name(node).c_str());
    ImGui::SameLine();
    ImGui::TextDisabled("(%s)", nodes_cfg.category_name(node).c_str());
    ImNodes::EndNodeTitleBar();

    bool const graph_has_changed = dropdown_to_switch_between_nodes_of_the_same_category(node, nodes_cfg, library, graph);

    draw_node_pins(node);
    draw_node_body(node, id, nodes_cfg);

    return graph_has_changed;
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::handle_link_creation() -> bool
{
    PinId from_pin_id;
    PinId to_pin_id;
    if (!ImNodes::IsLinkCreated(&from_pin_id, &to_pin_id))
        return false;

    _graph.remove_link_going_into(to_pin_id);
    _graph.add_link(Link{
        .from_pin_id = from_pin_id,
        .to_pin_id   = to_pin_id,
    });
    return true;
}

template<NodesCfg_Concept NodesCfg>
bool NodesEditor<NodesCfg>::handle_link_deletion()
{
    bool has_deleted_some = false;
    {
        ImNodes::ID link_id;
        if (ImNodes::IsLinkDestroyed(&link_id))
        {
            has_deleted_some = true;
            _graph.remove_link(link_id);
        }
    }

    {
        const int num_selected = ImNodes::NumSelectedLinks();
        if (num_selected > 0 && wants_to_delete_selection())
        {
            has_deleted_some           = true;
            static auto selected_links = std::vector<ImNodes::ID>{};
            selected_links.resize(static_cast<size_t>(num_selected));
            ImNodes::GetSelectedLinks(selected_links.data());
            for (auto const& link_id : selected_links)
                _graph.remove_link(link_id);
        }
    }

    return has_deleted_some;
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::handle_node_deletion() -> bool
{
    if (!wants_to_delete_selection())
        return false;

    const auto num_selected = ImNodes::NumSelectedNodes();
    if (num_selected == 0)
        return false;

    static auto selected_nodes = std::vector<ImNodes::ID>{};
    selected_nodes.resize(static_cast<size_t>(num_selected));
    ImNodes::GetSelectedNodes(selected_nodes.data());

    for (auto const& node_id : selected_nodes)
        _graph.remove_node(node_id);

    ImNodes::ClearNodeSelection();
    return true;
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::wants_to_delete_selection() const -> bool
{
    return _window_is_hovered
           && !ImGui::GetIO().WantTextInput
           && (ImGui::IsKeyReleased(ImGuiKey_Delete)
               || ImGui::IsKeyReleased(ImGuiKey_Backspace));
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::wants_to_open_nodes_menu() -> bool
{
    return _window_is_hovered
           && (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)
               || (ImGui::IsKeyReleased(ImGuiKey_A) && !ImGui::GetIO().WantTextInput)
           );
}

template<NodesCfg_Concept NodesCfg>
void NodesEditor<NodesCfg>::open_nodes_menu()
{
    ImGui::OpenPopup("_nodes_library");
    _search_bar.on_nodes_menu_open();

    _next_node_position = ImGui::GetMousePosOnOpeningCurrentPopup();
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::draw_nodes_library_menu_ifn(
    NodesCfg const&                                         nodes_cfg,
    NodesLibrary<typename NodesCfg::NodeDefinitionT> const& library
) -> bool
{
    bool b = false;

    bool menu_just_opened = false;
    if (wants_to_open_nodes_menu())
    {
        open_nodes_menu();
        menu_just_opened = true;
    }

    if (ImGui::BeginPopup("_nodes_library"))
    {
        if (imgui_nodes_menu(nodes_cfg, library, menu_just_opened))
        {
            ImGui::CloseCurrentPopup();
            b = true;
        }
        ImGui::EndPopup();
    }

    return b;
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::imgui_window(
    NodesCfg const&                                         nodes_cfg,
    NodesLibrary<typename NodesCfg::NodeDefinitionT> const& library
) -> bool
{
    ImNodes::SetCurrentContext(&*_context);
    bool graph_has_changed = false;
    ImGui::Begin("Nodes");
    _window_is_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy);
    graph_has_changed |= draw_nodes_library_menu_ifn(nodes_cfg, library);
    ImNodes::BeginNodeEditor();
    {
        {
            std::unique_lock lock{_graph.nodes().mutex()};
            for (auto& [id, node] : _graph.nodes())
            {
                auto const cat                        = library.get_category(node.category_name());
                auto const set_scoped_title_bar_color = ScopedTitleBarColor{
                    cat ? cat->config().get_color() : Color::from_srgb(glm::vec3{0.f}),
                };

                ImNodes::BeginNode(id);
                graph_has_changed |= draw_node<NodesCfg>(node, id, nodes_cfg, library, _graph);
                ImNodes::EndNode();
            }
        }
        {
            std::shared_lock lock{_graph.links().mutex()};
            for (auto const& [id, link] : _graph.links())
            {
                ImNodes::Link(id, link.from_pin_id, link.to_pin_id);
            }
        }
    }
    ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    ImNodes::EndNodeEditor();
    graph_has_changed |= handle_link_creation();
    graph_has_changed |= handle_link_deletion();
    graph_has_changed |= handle_node_deletion();
    ImGui::End();

    return graph_has_changed;
    // if (graph_has_changed)
    // {
    //     on_graph_change();
    // }
}

template<NodesCfg_Concept NodesCfg>
auto NodesEditor<NodesCfg>::
    imgui_nodes_menu(
        NodesCfg const&                                         nodes_cfg,
        NodesLibrary<typename NodesCfg::NodeDefinitionT> const& library,
        bool                                                    menu_just_opened
    ) -> bool
{
    bool const should_select_first_node   = _search_bar.imgui_widget();
    bool       should_open_all_categories = ImGui::IsItemEdited();

    auto const maybe_node_definition_id = library.imgui_nodes_menu(_search_bar.get_nodes_filter(), should_select_first_node, should_open_all_categories, menu_just_opened);
    if (!maybe_node_definition_id)
        return false;

    const auto id = _graph.add_node(nodes_cfg.make_node(*maybe_node_definition_id));
    ImNodes::SetNodeScreenSpacePos(id, _next_node_position);

    return true;
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

//         // waiting for xcode to support std::ranges::find_if
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

} // namespace Cool