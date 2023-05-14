
#include "Cool/Nodes/EditorImpl.h"
#include <imgui-node-editor/imgui_node_editor.h>
#include <reg/src/AnyId.hpp>
#include "Cool/Nodes/NodesConfig.h"
#include "Cool/Nodes/NodesLibrary.h"
#include "Cool/Nodes/as_reg_id.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Cool/ImGui/icon_fmt.h>
#include <imgui.h>
#include <imgui/imgui_internal.h>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "EditorImpl.h"
#include "as_ed_id.h"

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

// static auto draw_node(Cool::Node& node, ed::NodeId const& id, NodesConfig const& nodes_cfg, NodesLibrary const& library, Graph& graph) -> bool
// {
//     auto* drawList = ed::GetNodeBackgroundDrawList(id);
//     drawList->AddRectFilled({0.f, 0.f}, {100.f, 100.f}, 0xFFFFFFFF);
//     // ed::BeginNodeTitleBar();
//     // ImGui::TextUnformatted(nodes_cfg.name(node).c_str());
//     // ed::EndNodeTitleBar();

//     // if (ImGui::BeginPopupContextItem())
//     // {
//     //     nodes_cfg.widget_to_rename_node(node);
//     //     ImGui::EndPopup();
//     // }

//     // bool const graph_has_changed = dropdown_to_switch_between_nodes_of_the_same_category(node, nodes_cfg, library, graph);

//     // draw_node_body(node, id, nodes_cfg);
//     draw_node_pins(node);

//     // return graph_has_changed;
//     return false;
// }

auto NodesEditorImpl::handle_link_deletion() -> bool
{
    bool has_deleted_some = false;
    // {
    //     ed::ID link_id;
    //     if (ed::IsLinkDestroyed(&link_id))
    //     {
    //         has_deleted_some = true;
    //         _graph.remove_link(link_id);
    //     }
    // }

    // {
    //     int const num_selected = ed::NumSelectedLinks();
    //     if (num_selected > 0 && wants_to_delete_selection())
    //     {
    //         has_deleted_some           = true;
    //         static auto selected_links = std::vector<ed::ID>{};
    //         selected_links.resize(static_cast<size_t>(num_selected));
    //         ed::GetSelectedLinks(selected_links.data());
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

    // const auto num_selected = ed::NumSelectedNodes();
    // if (num_selected == 0)
    //     return false;

    // static auto selected_nodes = std::vector<ed::ID>{};
    // selected_nodes.resize(static_cast<size_t>(num_selected));
    // ed::GetSelectedNodes(selected_nodes.data());

    // for (auto const& node_id : selected_nodes)
    //     _graph.remove_node(node_id);

    // ed::ClearNodeSelection();
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
    ImGui::OpenPopup("_nodes_library");
    ed::Resume();
}

auto NodesEditorImpl::imgui_window_workspace(
    NodesConfig const&  nodes_cfg,
    NodesLibrary const& library
) -> bool
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 0.f});
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
    bool const should_render_window = ImGui::Begin(icon_fmt("Nodes", ICOMOON_TREE).c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
    ImGui::PopStyleVar(2);
    auto const prev_tesselation                  = ImGui::GetStyle().CircleTessellationMaxError;
    ImGui::GetStyle().CircleTessellationMaxError = 0.1f; // Make borders smooth even when zooming.

    if (should_render_window)
        OnFrame(nodes_cfg, library);

    ImGui::GetStyle().CircleTessellationMaxError = prev_tesselation;
    ImGui::End();

    return false;
}

static auto get_selected_nodes_ids(Graph const& graph) -> std::vector<NodeId>
{
    // Get ed IDs
    std::vector<ed::NodeId> nodes;
    nodes.resize(static_cast<size_t>(ed::GetSelectedObjectCount()));
    auto const nodes_count = ed::GetSelectedNodes(nodes.data(), static_cast<int>(nodes.size()));
    nodes.resize(static_cast<size_t>(nodes_count));

    // Convert to our IDs
    auto res = std::vector<NodeId>{};
    for (auto const& ed_id : nodes)
        res.push_back(as_reg_id(ed_id, graph));
    return res;
}

static void imgui_node_body(Node& node, NodeId const& id, NodesConfig const& nodes_cfg)
{
    ImGui::SeparatorText(node.definition_name().c_str());
    ImGui::PushID(&node);
    nodes_cfg.imgui_node_body(node, id);
    ImGui::PopID();
}

static auto imgui_selected_nodes(NodesConfig const& nodes_cfg, Graph& graph)
{
    auto const selected_nodes_ids = get_selected_nodes_ids(graph);
    for (auto const& node_id : selected_nodes_ids)
    {
        auto* node = graph.nodes().get_mutable_ref(node_id);
        if (!node)
            continue;
        imgui_node_body(*node, node_id, nodes_cfg);
        ImGui::NewLine();
    }
}

auto NodesEditorImpl::imgui_window_inspector(NodesConfig const& nodes_cfg) -> bool
{
    if (ImGui::Begin(icon_fmt("Inspector", ICOMOON_EQUALIZER).c_str()))
    {
        imgui_selected_nodes(nodes_cfg, _graph);
    }
    ImGui::End();
    return false;
}

auto NodesEditorImpl::imgui_windows(
    NodesConfig const&  nodes_cfg,
    NodesLibrary const& library
) -> bool
{
    bool b = false;
    b |= imgui_window_workspace(nodes_cfg, library);
    b |= imgui_window_inspector(nodes_cfg);
    return b;

    //

    // Cool::DebugOptions::nodes_style_editor([&]() {
    //     style_editor();
    // });
    // _window_is_hovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows | ImGuiHoveredFlags_NoPopupHierarchy);
    // ed::SetCurrentEditor(&*_context);
    // ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    // bool graph_has_changed = false;
    // // std::unique_lock lock{_graph.nodes().mutex()};
    // int uniqueId = 1;
    // for (auto& [id, node] : _graph.nodes())
    // {
    //     // auto const cat                        = library.get_category(node.category_name());
    //     // auto const set_scoped_title_bar_color = ScopedTitleBarColor{
    //     //     cat ? cat->config().get_color() : Color::from_srgb(glm::vec3{0.f}),
    //     // };

    //     auto const imnode_id = ed::NodeId{&node};
    //     ed::BeginNode(imnode_id);
    //     ed::EndNode();
    //     graph_has_changed |= draw_node(node, imnode_id, nodes_cfg, library, _graph);
    // }
    // ed::End();
    // ed::SetCurrentEditor(nullptr);
    // // ed::SetCurrentContext(&*_context);

    // graph_has_changed |= draw_nodes_library_menu_ifn(nodes_cfg, library);
    // // ed::BeginNodeEditor();
    // // {
    // //     std::shared_lock lock{_graph.links().mutex()};
    // //     for (auto const& [id, link] : _graph.links())
    // //     {
    // //         ed::Link(id, link.from_pin_id, link.to_pin_id);
    // //     }
    // // }
    // // ed::MiniMap(0.2f, edMiniMapLocation_BottomRight);
    // // ed::EndNodeEditor();
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
) -> NodeId
{
    bool const should_select_first_node   = _search_bar.imgui_widget();
    bool       should_open_all_categories = ImGui::IsItemEdited();

    auto const maybe_node_definition_id = library.imgui_nodes_menu(_search_bar.get_nodes_filter(), should_select_first_node, should_open_all_categories, menu_just_opened);
    if (!maybe_node_definition_id)
        return {};

    auto const id = _graph.add_node(nodes_cfg.make_node(*maybe_node_definition_id));
    return id;
}

static inline ImRect ImGui_GetItemRect()
{
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
    auto result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
}

static void show_label(const char* label, ImColor color)
{
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
    auto size = ImGui::CalcTextSize(label);

    auto padding = ImGui::GetStyle().FramePadding;
    auto spacing = ImGui::GetStyle().ItemSpacing;

    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

    auto rectMin = ImGui::GetCursorScreenPos() - padding;
    auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

    auto drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
    ImGui::TextUnformatted(label);
}

auto NodesEditorImpl::FindPin(ed::PinId const& id) -> Pin const*
{
    if (!id)
        return nullptr;

    for (auto const& [_, node] : _graph.nodes())
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

auto NodesEditorImpl::is_allowed_connection(Pin const& a, Pin const& b) -> bool
{
    return &a != &b
           && a.kind() != b.kind(); /*&& a->Type == b->Type && a->Node != b->Node */
}

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

void NodesEditorImpl::DrawPinIcon(Pin const&, bool connected, float alpha)
{
    ax::Widgets::IconType icon_type = ax::Widgets::IconType::Flow;
    ImColor               color     = ImColor{1.f, 1.f, 1.f, alpha}; // GetIconColor(pin.Type);

    ax::Widgets::Icon(ImVec2(24.f, 24.f), icon_type, connected, color, ImColor(0.125f, 0.125f, 0.125f, alpha));
};

void NodesEditorImpl::render_blueprint_node(Node& node, NodeId const& id, NodesCategory const* category, NodesConfig const& nodes_cfg, util::BlueprintNodeBuilder& builder)
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

    auto const pin_alpha = [&](Pin const& pin) {
        auto tmp_alpha = ImGui::GetStyle().Alpha;
        if (newLinkPin && !is_allowed_connection(*newLinkPin, pin))
            tmp_alpha *= 0.188f;
        return tmp_alpha;
    };

    for (auto& input_pin : node.input_pins())
    {
        auto const pin_id = as_ed_id(input_pin.id());
        auto const alpha  = pin_alpha(input_pin);

        builder.Input(pin_id);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        DrawPinIcon(input_pin, false /* IsPinLinked(pin_id) */, alpha);
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
        DrawPinIcon(output_pin, false /* IsPinLinked(pin_id) */, alpha);
        ImGui::PopStyleVar();
        builder.EndOutput();
    }

    // nodes_cfg.imgui_node_body(node, id);

    builder.End();
}

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

auto NodesEditorImpl::handle_link_creation() -> bool
{
    ed::PinId startPinId;
    ed::PinId endPinId;
    if (!ed::QueryNewLink(&startPinId, &endPinId))
        return false;

    auto const* startPin = FindPin(startPinId);
    auto const* endPin   = FindPin(endPinId);

    newLinkPin = startPin ? startPin : endPin;

    if (startPin && startPin->kind() == PinKind::Input)
    {
        std::swap(startPin, endPin);
        std::swap(startPinId, endPinId);
    }

    if (endPin == startPin)
    {
        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
        return false;
    }
    if (endPin->kind() == startPin->kind())
    {
        show_label("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
        return false;
    }
    // else if (endPin->Node == startPin->Node)
    // {
    //     show_label("x Cannot connect to self", ImColor(45, 32, 32, 180));
    //     ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
    // return false;
    // }
    // else if (endPin->Type != startPin->Type)
    // {
    //     show_label("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
    //     ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
    // return false;
    // }

    show_label("+ Create Link", ImColor(32, 45, 32, 180));
    if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
    {
        _graph.remove_link_going_into(endPin->id());
        _graph.add_link(Link{
            .from_pin_id = startPin->id(),
            .to_pin_id   = endPin->id(),
        });
    }

    return true;
}

void NodesEditorImpl::render_new_node()
{
    ed::PinId pinId = 0;
    if (ed::QueryNewNode(&pinId))
    {
        newLinkPin = FindPin(pinId);
        if (newLinkPin)
            show_label("+ Create Node", ImColor(32, 45, 32, 180));

        if (ed::AcceptNewItem())
        {
            newNodeLinkPin = FindPin(pinId);
            newLinkPin     = nullptr;
            open_nodes_menu();
        }
    }
}

void NodesEditorImpl::handle_creations()
{
    if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        handle_link_creation();
        render_new_node();
    }
    else
        newLinkPin = nullptr;

    ed::EndCreate();
}

void NodesEditorImpl::process_deletions()
{
    auto scope_guard = sg::make_scope_guard([] { ed::EndDelete(); });
    if (!ed::BeginDelete())
        return;

    {
        ed::LinkId link_id = 0;
        while (ed::QueryDeletedLink(&link_id))
        {
            if (ed::AcceptDeletedItem())
                _graph.remove_link(as_reg_id(link_id, _graph));
        }
    }

    {
        ed::NodeId node_id = 0;
        while (ed::QueryDeletedNode(&node_id))
        {
            if (ed::AcceptDeletedItem())
                _graph.remove_node(as_reg_id(node_id, _graph));
        }
    }
}

void NodesEditorImpl::render_editor(NodesLibrary const& library, NodesConfig const& nodes_cfg)
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

    handle_creations();
    process_deletions();
}

void NodesEditorImpl::OnFrame(NodesConfig const& nodes_cfg, NodesLibrary const& library)
{
    ed::SetCurrentEditor(_context);
    // GImGui->CurrentWindow->DrawList->AddRectFilled(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f},  ImGui::GetColorU32(ImGuiCol_SeparatorHovered), 0.0f); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume
    GImGui->CurrentWindow->DrawList->AddLine(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f}, ImGui::GetColorU32(ImGuiCol_SeparatorHovered)); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume

    ed::Begin("Node editor");
    {
        render_editor(library, nodes_cfg);
    }

    if (wants_to_open_nodes_menu())
    {
        open_nodes_menu();
        newNodeLinkPin = nullptr;
    }

    ed::Suspend();
    if (ImGui::BeginPopup("_nodes_library"))
    {
        auto const new_node_id = imgui_nodes_menu(nodes_cfg, library, _menu_just_opened);
        _menu_just_opened      = false;

        if (!new_node_id.underlying_uuid().is_nil())
        {
            ImGui::CloseCurrentPopup();

            ed::SetNodePosition(as_ed_id(new_node_id), _next_node_position);

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
    ed::Resume();
    ed::End();
}

} // namespace Cool