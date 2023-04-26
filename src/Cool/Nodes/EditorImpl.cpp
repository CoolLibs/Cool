#include "EditorImpl.h"
#include <Cool/ImGui/icon_fmt.h>
#include <imgui.h>
#include <imgui/imgui_internal.h>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "blueprints-example.h"

namespace Cool {

namespace internal {

void SearchBarState::on_nodes_menu_open()
{
    _nodes_filter.clear();
    _should_be_focused = true;
}

auto SearchBarState::get_nodes_filter() const -> std::string const&
{
    return _nodes_filter;
}

auto SearchBarState::imgui_widget() -> bool
{
    if (_should_be_focused)
    {
        ImGui::SetKeyboardFocusHere();
        _should_be_focused = false;
    }
    ImGui::PushID(868686);
    bool const b = ImGui::InputTextWithHint("##Filter", icon_fmt("Search for a node or category", ICOMOON_SEARCH).c_str(), &_nodes_filter, ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::PopID();
    return b;
}

} // namespace internal

static void draw_node_pins(Node const& node)
{
    ImGui::BeginGroup();
    for (auto const& pin : node.input_pins())
        pin.show();
    ImGui::EndGroup();

    ImGui::SameLine();

    ImGui::BeginGroup();
    for (auto const& pin : node.output_pins())
        pin.show();
    ImGui::EndGroup();
}

static void draw_node_body(Node& node, NodeId const& id, NodesConfig const& nodes_cfg)
{
    ImGui::BeginGroup();
    ImGui::PushID(&node);
    ImGui::PushItemWidth(200.f); // TODO(JF) Don't use a hardcoded value

    nodes_cfg.imgui_node_body(node, id);

    ImGui::PopItemWidth();
    ImGui::PopID();
    ImGui::EndGroup();
}

static auto calc_max_text_width(std::vector<NodeDefinition> const& defs) -> float
{
    float max = 0.f;
    for (auto const& def : defs)
        max = std::max(max, ImGui::CalcTextSize(def.name().c_str()).x);
    return max + 20.f;
}

static auto dropdown_to_switch_between_nodes_of_the_same_category(Cool::Node& node, NodesConfig const& nodes_cfg, NodesLibrary const& library, Graph& graph) -> bool
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

static auto draw_node(Cool::Node& node, ImNodes::NodeId const& id, NodesConfig const& nodes_cfg, NodesLibrary const& library, Graph& graph) -> bool
{
    auto* drawList = ImNodes::GetNodeBackgroundDrawList(id);
    drawList->AddRectFilled({0.f, 0.f}, {100.f, 100.f}, 0xFFFFFFFF);
    // ImNodes::BeginNodeTitleBar();
    // ImGui::TextUnformatted(nodes_cfg.name(node).c_str());
    // ImNodes::EndNodeTitleBar();

    // if (ImGui::BeginPopupContextItem())
    // {
    //     nodes_cfg.widget_to_rename_node(node);
    //     ImGui::EndPopup();
    // }

    // bool const graph_has_changed = dropdown_to_switch_between_nodes_of_the_same_category(node, nodes_cfg, library, graph);

    // draw_node_body(node, id, nodes_cfg);
    draw_node_pins(node);

    // return graph_has_changed;
    return false;
}

auto NodesEditorImpl::handle_link_creation() -> bool
{
    // PinId from_pin_id;
    // PinId to_pin_id;
    // if (!ImNodes::IsLinkCreated(&from_pin_id, &to_pin_id))
    //     return false;

    // _graph.remove_link_going_into(to_pin_id);
    // _graph.add_link(Link{
    //     .from_pin_id = from_pin_id,
    //     .to_pin_id   = to_pin_id,
    // });
    return true;
}

auto NodesEditorImpl::handle_link_deletion() -> bool
{
    bool has_deleted_some = false;
    // {
    //     ImNodes::ID link_id;
    //     if (ImNodes::IsLinkDestroyed(&link_id))
    //     {
    //         has_deleted_some = true;
    //         _graph.remove_link(link_id);
    //     }
    // }

    // {
    //     int const num_selected = ImNodes::NumSelectedLinks();
    //     if (num_selected > 0 && wants_to_delete_selection())
    //     {
    //         has_deleted_some           = true;
    //         static auto selected_links = std::vector<ImNodes::ID>{};
    //         selected_links.resize(static_cast<size_t>(num_selected));
    //         ImNodes::GetSelectedLinks(selected_links.data());
    //         for (auto const& link_id : selected_links)
    //             _graph.remove_link(link_id);
    //     }
    // }

    return has_deleted_some;
}

auto NodesEditorImpl::handle_node_deletion() -> bool
{
    // if (!wants_to_delete_selection())
    //     return false;

    // const auto num_selected = ImNodes::NumSelectedNodes();
    // if (num_selected == 0)
    //     return false;

    // static auto selected_nodes = std::vector<ImNodes::ID>{};
    // selected_nodes.resize(static_cast<size_t>(num_selected));
    // ImNodes::GetSelectedNodes(selected_nodes.data());

    // for (auto const& node_id : selected_nodes)
    //     _graph.remove_node(node_id);

    // ImNodes::ClearNodeSelection();
    return true;
}

auto NodesEditorImpl::wants_to_delete_selection() const -> bool
{
    return _window_is_hovered
           && !ImGui::GetIO().WantTextInput
           && (ImGui::IsKeyReleased(ImGuiKey_Delete)
               || ImGui::IsKeyReleased(ImGuiKey_Backspace));
}

auto NodesEditorImpl::wants_to_open_nodes_menu() const -> bool
{
    return _window_is_hovered
           && (ImGui::IsMouseReleased(ImGuiMouseButton_Middle)
               || (ImGui::IsKeyReleased(ImGuiKey_A) && !ImGui::GetIO().WantTextInput)
           );
}

void NodesEditorImpl::open_nodes_menu()
{
    ImGui::OpenPopup("_nodes_library");
    _search_bar.on_nodes_menu_open();

    _next_node_position = ImGui::GetMousePosOnOpeningCurrentPopup();
}

auto NodesEditorImpl::draw_nodes_library_menu_ifn(
    NodesConfig const&  nodes_cfg,
    NodesLibrary const& library
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

auto NodesEditorImpl::imgui_window(
    NodesConfig const&  nodes_cfg,
    NodesLibrary const& library
) -> bool
{
    ImGui::Begin(icon_fmt("Nodes", ICOMOON_TREE).c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
    auto const prev_tesselation                  = ImGui::GetStyle().CircleTessellationMaxError;
    ImGui::GetStyle().CircleTessellationMaxError = 0.1f; // Make borders smooth even when zooming.
    blueprints_example();
    ImGui::GetStyle().CircleTessellationMaxError = prev_tesselation;
    ImGui::End();

    // Cool::DebugOptions::nodes_style_editor([&]() {
    //     style_editor();
    // });
    return false;
    // _window_is_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy);
    // ImNodes::SetCurrentEditor(&*_context);
    // ImNodes::Begin("My Editor", ImVec2(0.0, 0.0f));
    // bool graph_has_changed = false;
    // // std::unique_lock lock{_graph.nodes().mutex()};
    // int uniqueId = 1;
    // for (auto& [id, node] : _graph.nodes())
    // {
    //     // auto const cat                        = library.get_category(node.category_name());
    //     // auto const set_scoped_title_bar_color = ScopedTitleBarColor{
    //     //     cat ? cat->config().get_color() : Color::from_srgb(glm::vec3{0.f}),
    //     // };

    //     auto const imnode_id = ImNodes::NodeId{&node};
    //     ImNodes::BeginNode(imnode_id);
    //     ImNodes::EndNode();
    //     graph_has_changed |= draw_node(node, imnode_id, nodes_cfg, library, _graph);
    // }
    // ImNodes::End();
    // ImNodes::SetCurrentEditor(nullptr);
    // // ImNodes::SetCurrentContext(&*_context);

    // graph_has_changed |= draw_nodes_library_menu_ifn(nodes_cfg, library);
    // // ImNodes::BeginNodeEditor();
    // // {
    // //     std::shared_lock lock{_graph.links().mutex()};
    // //     for (auto const& [id, link] : _graph.links())
    // //     {
    // //         ImNodes::Link(id, link.from_pin_id, link.to_pin_id);
    // //     }
    // // }
    // // ImNodes::MiniMap(0.2f, ImNodesMiniMapLocation_BottomRight);
    // // ImNodes::EndNodeEditor();
    // // graph_has_changed |= handle_link_creation();
    // // graph_has_changed |= handle_link_deletion();
    // // graph_has_changed |= handle_node_deletion();
    // ImGui::End();

    // return graph_has_changed;
}

auto NodesEditorImpl::imgui_nodes_menu(
    NodesConfig const&  nodes_cfg,
    NodesLibrary const& library,
    bool                menu_just_opened
) -> bool
{
    bool const should_select_first_node   = _search_bar.imgui_widget();
    bool       should_open_all_categories = ImGui::IsItemEdited();

    auto const maybe_node_definition_id = library.imgui_nodes_menu(_search_bar.get_nodes_filter(), should_select_first_node, should_open_all_categories, menu_just_opened);
    if (!maybe_node_definition_id)
        return false;

    auto const id = _graph.add_node(nodes_cfg.make_node(*maybe_node_definition_id));
    // ImNodes::SetNodeScreenSpacePos(id, _next_node_position);

    return true;
}

} // namespace Cool