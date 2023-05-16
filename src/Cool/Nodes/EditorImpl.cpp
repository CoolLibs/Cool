
#include "EditorImpl.h"
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/icon_fmt.h"
#include "EditorImpl.h"
#include "Graph.h"
#include "NodesConfig.h"
#include "NodesLibrary.h"
#include "as_ed_id.h"
#include "as_reg_id.h"
#include "imgui-node-editor/imgui_node_editor.h"
#include "reg/src/AnyId.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui.h>
#include <imgui/imgui_internal.h>

namespace Cool {

namespace internal {

void SearchBarState::on_nodes_menu_open()
{
    _nodes_filter.clear();
    _should_be_focused = true;
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

static auto imgui_all_definitions_selectables(Node& node, NodesCategory const& category, NodesConfig& nodes_cfg, Graph& graph)
    -> bool
{
    bool graph_has_changed = false;

    for (auto const& def : category.definitions())
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

    return graph_has_changed;
}

static auto dropdown_to_switch_between_nodes_of_the_same_category(Cool::Node& node, NodesConfig& nodes_cfg, NodesLibrary const& library, Graph& graph) -> bool
{
    auto const* category = library.get_category(node.category_name());
    if (!category)
        return false;

    bool graph_has_changed = false;

    if (ImGui::BeginCombo(category->name().c_str(), node.definition_name().c_str()))
    {
        graph_has_changed |= imgui_all_definitions_selectables(node, *category, nodes_cfg, graph);
        ImGui::EndCombo();
    }

    return graph_has_changed;
}

[[nodiscard]] static auto wants_to_delete_selection() -> bool
{
    return !ImGui::GetIO().WantTextInput
           && (ImGui::IsKeyReleased(ImGuiKey_Delete)
               || ImGui::IsKeyReleased(ImGuiKey_Backspace)
           );
}

auto NodesEditorImpl::wants_to_open_nodes_menu() const -> bool
{
    return _workspace_is_hovered
           && (ed::ShowBackgroundContextMenu()
               || (ImGui::IsKeyReleased(ImGuiKey_A) && !ImGui::GetIO().WantTextInput)
           );
}

void NodesEditorImpl::open_nodes_menu()
{
    _menu_just_opened   = true;
    _next_node_position = ImGui::GetMousePos();
    _search_bar.on_nodes_menu_open();
    ed::Suspend();
    ImGui::OpenPopup("Nodes Library Menu");
    ed::Resume();
}

auto NodesEditorImpl::imgui_window_workspace(
    NodesConfig&        nodes_cfg,
    NodesLibrary const& library
) -> bool
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 0.f});
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
    bool const should_render_window = ImGui::Begin(icon_fmt("Nodes", ICOMOON_TREE).c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
    ImGui::PopStyleVar(2);
    auto const prev_tesselation                  = ImGui::GetStyle().CircleTessellationMaxError;
    ImGui::GetStyle().CircleTessellationMaxError = 0.1f; // Make borders smooth even when zooming.

    bool graph_has_changed = false;
    if (should_render_window)
        graph_has_changed |= imgui_workspace(nodes_cfg, library);

    ImGui::GetStyle().CircleTessellationMaxError = prev_tesselation;
    _workspace_is_hovered                        = ImGui::IsWindowHovered();
    ImGui::End();

    return graph_has_changed;
}

static auto get_selected_nodes_ids() -> std::vector<ed::NodeId>
{
    auto nodes = std::vector<ed::NodeId>{};
    nodes.resize(static_cast<size_t>(ed::GetSelectedObjectCount()));
    auto const nodes_count = ed::GetSelectedNodes(nodes.data(), static_cast<int>(nodes.size()));
    nodes.resize(static_cast<size_t>(nodes_count));
    return nodes;
}

static auto get_selected_links_ids() -> std::vector<ed::LinkId>
{
    auto links = std::vector<ed::LinkId>{};
    links.resize(static_cast<size_t>(ed::GetSelectedObjectCount()));
    auto const links_count = ed::GetSelectedLinks(links.data(), static_cast<int>(links.size()));
    links.resize(static_cast<size_t>(links_count));
    return links;
}

static auto imgui_node_in_inspector(Node& node, NodeId const& id, NodesConfig& nodes_cfg, NodesLibrary const& library, Graph& graph)
    -> bool
{
    ImGui::SeparatorText(node.definition_name().c_str());
    ImGui::PushID(&node);
    bool const graph_has_changed =
        dropdown_to_switch_between_nodes_of_the_same_category(node, nodes_cfg, library, graph);
    nodes_cfg.imgui_node_in_inspector(node, id);
    ImGui::PopID();
    return graph_has_changed;
}

auto NodesEditorImpl::imgui_window_inspector(NodesConfig& nodes_cfg, NodesLibrary const& library) -> bool
{
    bool graph_has_changed = false;
    if (ImGui::Begin(icon_fmt("Inspector", ICOMOON_EQUALIZER).c_str()))
    {
        auto const selected_nodes_ids = get_selected_nodes_ids();

        // Message when no node is selected
        if (selected_nodes_ids.empty())
        {
            ImGui::PushFont(Font::italic());
            ImGui::TextUnformatted("Select a node to edit its parameters.");
            ImGui::PopFont();
        }

        // Show all nodes
        for (auto const& ed_node_id : selected_nodes_ids)
        {
            auto const node_id = as_reg_id(ed_node_id, _graph);

            auto* node = _graph.nodes().get_mutable_ref(node_id);
            if (!node)
                continue;
            graph_has_changed |= imgui_node_in_inspector(*node, node_id, nodes_cfg, library, _graph);
            ImGui::NewLine();
        }
    }
    ImGui::End();
    return graph_has_changed;
}

auto NodesEditorImpl::imgui_windows(
    NodesConfig&        nodes_cfg,
    NodesLibrary const& library
) -> bool
{
    bool graph_has_changed = false;
    graph_has_changed |= imgui_window_workspace(nodes_cfg, library);
    graph_has_changed |= imgui_window_inspector(nodes_cfg, library);
    return graph_has_changed;
}

auto NodesEditorImpl::imgui_nodes_menu(NodesLibrary const& library, bool menu_just_opened)
    -> std::optional<NodeDefinitionAndCategoryName>
{
    bool const should_select_first_node   = _search_bar.imgui_widget();
    bool       should_open_all_categories = ImGui::IsItemEdited();

    return library.imgui_nodes_menu(_search_bar.get_nodes_filter(), should_select_first_node, should_open_all_categories, menu_just_opened);
}

static auto find_pin(ed::PinId const& id, Graph const& graph) -> Pin const*
{
    if (!id)
        return nullptr;

    for (auto const& [_, node] : graph.nodes())
    {
        for (auto const& pin : node.input_pins())
            if (ed::PinId{as_ed_id(pin.id())} == id)
                return &pin;

        for (auto const& pin : node.output_pins())
            if (ed::PinId{as_ed_id(pin.id())} == id)
                return &pin;
    }

    return nullptr;
}

static auto is_allowed_connection(Pin const& a, Pin const& b, Graph const& graph) -> bool
{
    auto const node_id_a = GraphU::node_id(graph, a.id());
    auto const node_id_b = GraphU::node_id(graph, b.id());
    return &a != &b
           && a.kind() != b.kind()
           && node_id_a != node_id_b; /*&& a->Type == b->Type  */
}

// TODO(JF)
// NodeEX* NodesEditorImpl::SpawnComment()
// {
//     m_Nodes.emplace_back(GetNextId(), "Test Comment");
//     m_Nodes.back().Type = NodeType::Comment;
//     m_Nodes.back().Size = ImVec2(300, 200);

//     return &m_Nodes.back();
// }

// ImColor NodesEditorImpl::GetIconColor(PinType type)
// {
//     switch (type)
//     {
//     default:
//     case PinType::Flow: return ImColor(255, 255, 255);
//     case PinType::Bool: return ImColor(220, 48, 48);
//     case PinType::Int: return ImColor(68, 201, 156);
//     case PinType::Float: return ImColor(147, 226, 74);
//     case PinType::String: return ImColor(124, 21, 153);
//     case PinType::Object: return ImColor(51, 150, 215);
//     case PinType::Function: return ImColor(218, 0, 183);
//     case PinType::Delegate: return ImColor(255, 48, 48);
//     }
// };

static void draw_pin_icon(Pin const&, bool connected, float alpha)
{
    ax::Widgets::IconType icon_type = ax::Widgets::IconType::Circle;
    ImColor               color     = ImColor{1.f, 1.f, 1.f, alpha}; // GetIconColor(pin.Type);

    ax::Widgets::Icon(ImVec2(24.f, 24.f), icon_type, connected, color, ImColor(0.125f, 0.125f, 0.125f, alpha));
};

void NodesEditorImpl::render_blueprint_node(Node& node, NodeId const& id, NodesCategory const* category, NodesConfig& nodes_cfg, util::BlueprintNodeBuilder& builder)
{
    builder.Begin(as_ed_id(id));

    auto const color = category ? category->config().get_color() : Color::from_srgb(glm::vec3{0.f});
    builder.Header(color.as_ImColor());
    ImGui::Spring(0);
    ImGui::TextUnformatted(node.definition_name().c_str());
    ImGui::Spring(1);
    ImGui::Dummy(ImVec2(0, 28));
    ImGui::Spring(0);
    builder.EndHeader();

    nodes_cfg.imgui_above_node_pins(node, id);

    // Begin pins
    ImGui::BeginHorizontal("pins");
    ImGui::Spring(0, 0);

    auto const pin_alpha = [&](Pin const& pin) {
        auto tmp_alpha = ImGui::GetStyle().Alpha;
        if (_new_link_pin && !is_allowed_connection(*_new_link_pin, pin, _graph))
            tmp_alpha *= 0.188f;
        return tmp_alpha;
    };

    for (auto& input_pin : node.input_pins())
    {
        auto const pin_id = as_ed_id(input_pin.id());
        auto const alpha  = pin_alpha(input_pin);

        builder.Input(pin_id);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        draw_pin_icon(input_pin, false /* IsPinLinked(pin_id) */, alpha);
        ImGui::Spring(0);
        if (!input_pin.name().empty())
        {
            ImGui::TextUnformatted(input_pin.name().c_str());
            ImGui::Spring(0);
        }
        ImGui::PopStyleVar();
        builder.EndInput();
    }

    for (auto& output_pin : node.output_pins())
    {
        auto const pin_id = as_ed_id(output_pin.id());
        auto const alpha  = pin_alpha(output_pin);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        builder.Output(pin_id);
        if (!output_pin.name().empty())
        {
            ImGui::Spring(0);
            ImGui::TextUnformatted(output_pin.name().c_str());
        }
        ImGui::Spring(0);
        draw_pin_icon(output_pin, false /* IsPinLinked(pin_id) */, alpha);
        ImGui::PopStyleVar();
        builder.EndOutput();
    }

    // End output pins
    ed::PopStyleVar(2);
    ImGui::Spring(1, 0);
    ImGui::EndVertical();

    // End pins
    ImGui::EndHorizontal();

    nodes_cfg.imgui_below_node_pins(node, id);

    builder.End();
}

// TODO(JF)
// void NodesEditorImpl::render_comment_node(NodeEX& node)
// {
//     const float commentAlpha = 0.75f;

//     ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
//     ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
//     ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
//     ed::BeginNode(node.ID);
//     ImGui::PushID(node.ID.AsPointer());
//     ImGui::BeginVertical("content");
//     ImGui::BeginHorizontal("horizontal");
//     ImGui::Spring(1);
//     ImGui::TextUnformatted(node.Name.c_str());
//     ImGui::Spring(1);
//     ImGui::EndHorizontal();
//     ed::Group(node.Size);
//     ImGui::EndVertical();
//     ImGui::PopID();
//     ed::EndNode();
//     ed::PopStyleColor(2);
//     ImGui::PopStyleVar();

//     if (ed::BeginGroupHint(node.ID))
//     {
//         // auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
//         auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

//         // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

//         auto min = ed::GetGroupMin();
//         // auto max = ed::GetGroupMax();

//         ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
//         ImGui::BeginGroup();
//         ImGui::TextUnformatted(node.Name.c_str());
//         ImGui::EndGroup();

//         auto drawList = ed::GetHintBackgroundDrawList();

//         auto hintBounds      = ImGui_GetItemRect();
//         auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

//         drawList->AddRectFilled(
//             hintFrameBounds.GetTL(),
//             hintFrameBounds.GetBR(),
//             IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f
//         );

//         drawList->AddRect(
//             hintFrameBounds.GetTL(),
//             hintFrameBounds.GetBR(),
//             IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f
//         );

//         // ImGui::PopStyleVar();
//     }
//     ed::EndGroupHint();
// }

auto NodesEditorImpl::process_link_creation() -> bool
{
    ed::PinId begin_pin_id;
    ed::PinId end_pin_id;
    if (!ed::QueryNewLink(&begin_pin_id, &end_pin_id))
        return false;

    auto const* begin_pin = find_pin(begin_pin_id, _graph);
    auto const* end_pin   = find_pin(end_pin_id, _graph);

    _new_link_pin = begin_pin ? begin_pin : end_pin;

    if (begin_pin && begin_pin->kind() == PinKind::Input)
    {
        std::swap(begin_pin, end_pin);
        std::swap(begin_pin_id, end_pin_id);
    }

    if (end_pin == begin_pin)
    {
        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
        return false;
    }
    if (end_pin->kind() == begin_pin->kind())
    {
        show_label("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
        return false;
    }
    // else if (end_pin->Node == startPin->Node)
    // {
    //     show_label("x Cannot connect to self", ImColor(45, 32, 32, 180));
    //     ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
    // return false;
    // }
    // else if (end_pin->Type != startPin->Type)
    // {
    //     show_label("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
    //     ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
    // return false;
    // }

    show_label("+ Create Link", ImColor(32, 45, 32, 180));
    if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
    {
        _graph.remove_link_going_into(end_pin->id());
        _graph.add_link(Link{
            .from_pin_id = begin_pin->id(),
            .to_pin_id   = end_pin->id(),
        });
    }

    return true;
}

void NodesEditorImpl::process_link_released()
{
    ed::PinId pinId = 0;
    if (ed::QueryNewNode(&pinId))
    {
        _new_link_pin = find_pin(pinId, _graph);
        if (_new_link_pin)
            show_label("+ Create Node", ImColor(32, 45, 32, 180));

        if (ed::AcceptNewItem())
        {
            _new_node_link_pin = find_pin(pinId, _graph);
            _new_link_pin      = nullptr;
            open_nodes_menu();
        }
    }
}

auto NodesEditorImpl::process_creations() -> bool
{
    bool graph_has_changed = false;

    if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        graph_has_changed |= process_link_creation();
        process_link_released();
    }
    else
        _new_link_pin = nullptr;

    ed::EndCreate();

    return graph_has_changed;
}

static auto process_deletions(Graph& graph, bool wants_to_delete_selection) -> bool
{
    bool graph_has_changed = false;
    {
        auto scope_guard = sg::make_scope_guard([] { ed::EndDelete(); });
        if (ed::BeginDelete())
        {
            {
                ed::LinkId link_id = 0;
                while (ed::QueryDeletedLink(&link_id))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        graph.remove_link(as_reg_id(link_id, graph));
                        graph_has_changed = true;
                    }
                }
            }
            {
                ed::NodeId node_id = 0;
                while (ed::QueryDeletedNode(&node_id))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        graph.remove_node(as_reg_id(node_id, graph));
                        graph_has_changed = true;
                    }
                }
            }
        }
    }

    if (wants_to_delete_selection)
    {
        {
            auto const links_ids = get_selected_links_ids();
            for (auto const& link_id : links_ids)
            {
                graph.remove_link(as_reg_id(link_id, graph));
                ed::DeleteLink(link_id);
                graph_has_changed = true;
            }
        }
        {
            auto const nodes_ids = get_selected_nodes_ids();
            for (auto const& node_id : nodes_ids)
            {
                graph.remove_node(as_reg_id(node_id, graph));
                ed::DeleteNode(node_id);
                graph_has_changed = true;
            }
        }
    }

    return graph_has_changed;
}

void NodesEditorImpl::render_editor(NodesLibrary const& library, NodesConfig& nodes_cfg)
{
    util::BlueprintNodeBuilder builder{};

    for (auto& [id, node] : _graph.nodes())
    {
        // if (node.Type == NodeType::Blueprint)
        render_blueprint_node(node, id, library.get_category(node.category_name()), nodes_cfg, builder);
        // if (node.Type == NodeType::Comment)
        //     render_comment_node(node);
    }

    for (auto const& [id, link] : _graph.links())
        ed::Link(as_ed_id(id), as_ed_id(link.from_pin_id), as_ed_id(link.to_pin_id), ImColor{1.f, 1.f, 1.f, 1.f}, 2.0f);
}

auto NodesEditorImpl::imgui_workspace(NodesConfig& nodes_cfg, NodesLibrary const& library) -> bool
{
    bool graph_has_changed = false;

    ed::SetCurrentEditor(_context);
    // GImGui->CurrentWindow->DrawList->AddRectFilled(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f},  ImGui::GetColorU32(ImGuiCol_SeparatorHovered), 0.0f); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume
    GImGui->CurrentWindow->DrawList->AddLine(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f}, ImGui::GetColorU32(ImGuiCol_SeparatorHovered)); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume

    ed::Begin("Node editor");

    render_editor(library, nodes_cfg);

    graph_has_changed |= process_creations();
    graph_has_changed |= process_deletions(_graph, wants_to_delete_selection());

    if (wants_to_open_nodes_menu())
    {
        open_nodes_menu();
        _new_node_link_pin = nullptr;
    }

    if (ed::ShowNodeContextMenu(&_id_of_node_whose_context_menu_is_open))
    {
        ed::Suspend();
        ImGui::OpenPopup("Node Context Menu");
        ed::Resume();
    }

    ed::Suspend();
    if (ImGui::BeginPopup("Nodes Library Menu"))
    {
        auto const new_node_def_id = imgui_nodes_menu(library, _menu_just_opened);
        _menu_just_opened          = false;

        if (new_node_def_id)
        {
            ImGui::CloseCurrentPopup();

            auto const new_node_id = _graph.add_node(nodes_cfg.make_node(*new_node_def_id));
            ed::SetNodePosition(as_ed_id(new_node_id), _next_node_position);

            // TODO(JF)
            // if (auto startPin = newNodeLinkPin)
            // {
            //     auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

            //     for (auto& pin : pins)
            //     {
            //         if (CanCreateLink(startPin, &pin))
            //         {
            //             auto endPin = &pin;
            //             if (startPin->Kind == PinKind::Input)
            //                 std::swap(startPin, endPin);

            //             m_Links.emplace_back(LinkEX(GetNextId(), startPin->ID, endPin->ID));
            //             m_Links.back().Color = GetIconColor(startPin->Type);

            //             break;
            //         }
            //     }
            // }
        }

        ImGui::EndPopup();
    }
    if (ImGui::BeginPopup("Node Context Menu"))
    {
        auto const id   = as_reg_id(_id_of_node_whose_context_menu_is_open, _graph);
        auto*      node = _graph.nodes().get_mutable_ref(id);
        if (node)
        {
            auto const* category = library.get_category(node->category_name());
            if (category)
                graph_has_changed |= imgui_all_definitions_selectables(*node, *category, nodes_cfg, _graph);
        }

        ImGui::EndPopup();
    }
    ed::Resume();
    ed::End();

    return graph_has_changed;
}

} // namespace Cool