#include "EditorImpl.h"
#include <imgui.h>
#include <imgui/imgui_internal.h>
#include <reg/src/internal/generate_uuid.hpp>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/Fonts.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "Cool/ImGui/ImGuiExtrasStyle.h"
#include "Cool/ImGui/icon_fmt.h"
#include "Cool/Nodes/EditorImpl.h"
#include "Cool/Nodes/as_ed_id.h"
#include "Cool/Nodes/utilities/drawing.h"
#include "Cool/StrongTypes/Color.h"
#include "EditorImpl.h"
#include "NodesConfig.h"
#include "NodesGraph.h"
#include "NodesLibrary.h"
#include "as_ed_id.h"
#include "as_reg_id.h"
#include "imgui-node-editor/imgui_node_editor.h"
#include "reg/src/AnyId.hpp"

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

FrameNode::FrameNode()
    : id{reg::internal::generate_uuid()}
    , name{"Frame"}
{
}

} // namespace internal

static auto imgui_all_definitions_selectables(Node& node, NodesCategory const& category, NodesConfig& nodes_cfg, NodesGraph& graph)
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
        if (is_selected) // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            ImGui::SetItemDefaultFocus();
        ImGui::PopID();
    }

    return graph_has_changed;
}

static auto dropdown_to_switch_between_nodes_of_the_same_category(Cool::Node& node, NodesConfig& nodes_cfg, NodesLibrary const& library, NodesGraph& graph) -> bool
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
           && ImGui::IsWindowFocused()
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
    ed::PushStyleColor(ed::StyleColor_SelLinkBorder, ImGuiExtras::GetStyle().link_hovered);
    ed::PushStyleColor(ed::StyleColor_HovLinkBorder, ImGuiExtras::GetStyle().link_hovered);
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
    ed::PopStyleColor(2);

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

static auto imgui_node_in_inspector(Node& node, NodeId const& id, NodesConfig& nodes_cfg, NodesLibrary const& library, NodesGraph& graph)
    -> bool
{
    ImGuiExtras::separator_text(node.definition_name());
    ImGui::PushID(&node);

    nodes_cfg.imgui_in_inspector_above_node_info(node, id);

    nodes_cfg.widget_to_rename_node(node);
    bool const graph_has_changed = dropdown_to_switch_between_nodes_of_the_same_category(node, nodes_cfg, library, graph);

    nodes_cfg.imgui_in_inspector_below_node_info(node, id);

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
            if (node)
            {
                graph_has_changed |= imgui_node_in_inspector(*node, node_id, nodes_cfg, library, _graph);
            }
            else // Frame node
            {
                for (auto& frame_node : _frame_nodes)
                {
                    if (ed::NodeId{as_ed_id(frame_node.id)} != ed_node_id)
                        continue;
                    ImGui::InputText("Title", &frame_node.name);
                }
            }
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

auto NodesEditorImpl::imgui_nodes_menu(NodesLibrary const& library, MaybeDisableNodeDefinition const& maybe_disable, bool menu_just_opened)
    -> std::optional<NodeDefinitionAndCategoryName>
{
    bool const should_select_first_node   = _search_bar.imgui_widget();
    bool       should_open_all_categories = ImGui::IsItemEdited();

    return library.imgui_nodes_menu(_search_bar.get_nodes_filter(), maybe_disable, should_select_first_node, should_open_all_categories, menu_just_opened);
}

static auto find_pin(ed::PinId const& id, NodesGraph const& graph) -> Pin const*
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

static auto is_allowed_connection(Pin const& a, Pin const& b, NodesGraph const& graph) -> bool
{
    auto const node_id_a = graph.find_node_containing_pin(a.id());
    auto const node_id_b = graph.find_node_containing_pin(b.id());
    return &a != &b
           && a.kind() != b.kind()
           && node_id_a != node_id_b; /*&& a->Type == b->Type  */
}

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

static void render_pin_icon(ax::Drawing::IconType icon, float alpha, Cool::Color color)
{
    auto const col = color.as_sRGB();
    ax::Widgets::Icon(ImVec2(24.f, 24.f), icon, true, {col.x, col.y, col.z, alpha}, ImColor(0.125f, 0.125f, 0.125f, alpha));
};

static auto input_pin_icon(size_t pin_index, Cool::NodesCategory const* category) -> ax::Drawing::IconType
{
    return category && pin_index < category->config().number_of_main_input_pins()
               ? ax::Drawing::IconType::Flow
               : ax::Drawing::IconType::Circle;
}

static auto output_pin_icon(size_t pin_index)
{
    return pin_index == 0
               ? ax::Drawing::IconType::Flow
               : ax::Drawing::IconType::Circle;
}

void NodesEditorImpl::render_node(Node& node, NodeId const& id, NodesConfig& nodes_cfg, NodesLibrary const& library, ax::NodeEditor::Utilities::BlueprintNodeBuilder& builder)
{
    auto const  color       = nodes_cfg.node_color(node, id);
    auto const* category    = library.get_category(node.category_name());
    auto const  title_color = get_text_color(color);

    ed::PushStyleColor(ed::StyleColor_SelNodeBorder, color.as_ImColor());
    builder.Begin(as_ed_id(id));
    ed::PopStyleColor();

    builder.Header(color.as_ImColor());
    ImGui::Spring(0);
    ImGui::PushFont(Font::bold());
    ImGui::PushStyleColor(ImGuiCol_Text, title_color.as_ImColor().Value);
    auto const node_name = nodes_cfg.name(node);
    ImGui::TextUnformatted((node_name.empty() ? node.definition_name() : node_name).c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();
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

    for (size_t idx = 0; idx < node.input_pins().size(); ++idx)
    {
        auto&      input_pin = node.input_pins()[idx];
        auto const pin_id    = as_ed_id(input_pin.id());
        auto const alpha     = pin_alpha(input_pin);

        builder.Input(pin_id);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        render_pin_icon(input_pin_icon(idx, category), alpha, nodes_cfg.pin_color(input_pin, idx, node, id));
        ImGui::Spring(0);
        if (!input_pin.name().empty())
        {
            ImGui::TextUnformatted(input_pin.name().c_str());
            ImGui::Spring(0);
        }
        ImGui::PopStyleVar();
        builder.EndInput();
    }

    for (size_t idx = 0; idx < node.output_pins().size(); ++idx)
    {
        auto&      output_pin = node.output_pins()[idx];
        auto const pin_id     = as_ed_id(output_pin.id());
        auto const alpha      = pin_alpha(output_pin);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        builder.Output(pin_id);
        if (!output_pin.name().empty())
        {
            ImGui::Spring(0);
            ImGui::TextUnformatted(output_pin.name().c_str());
        }
        ImGui::Spring(0);
        render_pin_icon(output_pin_icon(idx), alpha, nodes_cfg.pin_color(output_pin, idx, node, id));
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

static auto ImGui_GetItemRect() -> ImRect
{
    return ImRect{ImGui::GetItemRectMin(), ImGui::GetItemRectMax()};
}

static auto ImRect_Expanded(const ImRect& rect, float x, float y) -> ImRect
{
    auto result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
}

static void render_frame_node(internal::FrameNode const& node)
{
    // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.75f);
    ed::PushStyleColor(ed::StyleColor_NodeBg, ImGuiExtras::GetStyle().frame_node_color);
    ed::PushStyleColor(ed::StyleColor_NodeBorder, ImGuiExtras::GetStyle().frame_node_color);
    auto const id = ed::NodeId{as_ed_id(node.id)};
    ed::BeginNode(id);
    ImGui::PushID(id.AsPointer());
    ImGui::BeginVertical("content");
    ImGui::BeginHorizontal("horizontal");
    ImGui::Spring(1);
    ImGui::TextUnformatted(node.name.c_str());
    ImGui::Spring(1);
    ImGui::EndHorizontal();
    ed::Group({300.f, 200.f});
    ImGui::EndVertical();
    ImGui::PopID();
    ed::EndNode();
    ed::PopStyleColor(2);
    // ImGui::PopStyleVar();

    if (ed::BeginGroupHint(id))
    {
        // auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);

        // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

        auto min = ed::GetGroupMin();
        // auto max = ed::GetGroupMax();

        ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
        ImGui::BeginGroup();
        ImGui::TextUnformatted(node.name.c_str());
        ImGui::EndGroup();

        auto drawList = ed::GetHintBackgroundDrawList();

        auto hintBounds      = ImGui_GetItemRect();
        auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

        auto       frame_color = ImGuiExtras::GetStyle().frame_node_color;
        auto const bg_alpha    = ImGui::GetStyle().Alpha;

        frame_color.w = 64.f * bg_alpha / 255.f;
        drawList->AddRectFilled(
            hintFrameBounds.GetTL(),
            hintFrameBounds.GetBR(),
            ImColor{frame_color}, 4.0f
        );

        frame_color.w = 128.f * bg_alpha / 255.f;
        drawList->AddRect(
            hintFrameBounds.GetTL(),
            hintFrameBounds.GetBR(),
            ImColor{frame_color}, 4.0f
        );

        // ImGui::PopStyleVar();
    }
    ed::EndGroupHint();
}

auto NodesEditorImpl::process_link_creation(NodesConfig& nodes_cfg) -> bool
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
        ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
        return false;
    }
    // else if (end_pin->Node == startPin->Node)
    // {
    //     ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
    // return false;
    // }
    // else if (end_pin->Type != startPin->Type)
    // {
    //     ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
    // return false;
    // }

    if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
    {
        auto const link = Link{
            .from_pin_id = begin_pin->id(),
            .to_pin_id   = end_pin->id(),
        };
        auto const link_id = _graph.add_link(link);
        nodes_cfg.on_link_created_between_existing_nodes(link, link_id);
    }

    return true;
}

void NodesEditorImpl::process_link_released()
{
    ed::PinId pinId = 0;
    if (ed::QueryNewNode(&pinId))
    {
        _new_link_pin = find_pin(pinId, _graph);

        if (ed::AcceptNewItem())
        {
            _pin_to_link_to_new_node     = find_pin(pinId, _graph);
            _new_link_pin                = nullptr;
            _link_has_just_been_released = true;
        }
    }
}

auto NodesEditorImpl::process_creations(NodesConfig& nodes_cfg) -> bool
{
    bool graph_has_changed = false;

    if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        graph_has_changed |= process_link_creation(nodes_cfg);
        process_link_released();
    }
    else
        _new_link_pin = nullptr;

    ed::EndCreate();

    return graph_has_changed;
}

static void remove_frame_node(std::vector<internal::FrameNode>& frame_nodes, ed::NodeId id)
{
    frame_nodes.erase(std::remove_if(frame_nodes.begin(), frame_nodes.end(), [&](internal::FrameNode const& node) {
                          return ed::NodeId{as_ed_id(node.id)} == id;
                      }),
                      frame_nodes.end());
}

static auto process_deletions(NodesGraph& graph, std::vector<internal::FrameNode>& frame_nodes, bool wants_to_delete_selection) -> bool
{
    if (wants_to_delete_selection)
    {
        {
            auto const links_ids = get_selected_links_ids();
            for (auto const& link_id : links_ids)
                ed::DeleteLink(link_id); // This will trigger the ed::QueryDeletedLink() below.
        }
        {
            auto const nodes_ids = get_selected_nodes_ids();
            for (auto const& node_id : nodes_ids)
                ed::DeleteNode(node_id); // This will trigger the ed::QueryDeletedNode() below.
        }
    }

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
                        remove_frame_node(frame_nodes, node_id);
                        graph_has_changed = true;
                    }
                }
            }
        }
    }

    return graph_has_changed;
}

void NodesEditorImpl::render_editor(NodesConfig& nodes_cfg, NodesLibrary const& library)
{
    ax::NodeEditor::Utilities::BlueprintNodeBuilder builder{};
    for (auto& [id, node] : _graph.nodes())
        render_node(node, id, nodes_cfg, library, builder);

    for (auto const& frame_node : _frame_nodes)
        render_frame_node(frame_node);

    for (auto const& [id, link] : _graph.links())
        ed::Link(as_ed_id(id), as_ed_id(link.from_pin_id), as_ed_id(link.to_pin_id), ImGuiExtras::GetStyle().link, 2.0f);
}

auto NodesEditorImpl::imgui_workspace(NodesConfig& nodes_cfg, NodesLibrary const& library) -> bool
{
    {
        // HACK(ImNodes) To make sure the zoom level is properly restored as it was when we closed the app
        // we need to give one frame for all the other ImGui windows to render
        // and only start rendering the nodes' workspace during the second frame.
        // NB: go check out the other "HACK(ImNodes)"
        _frames_count++;
        if (_frames_count <= 1)
            return false;
    }

    bool graph_has_changed = false;

    ed::SetCurrentEditor(_context);
    // GImGui->CurrentWindow->DrawList->AddRectFilled(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f},  ImGui::GetColorU32(ImGuiCol_SeparatorHovered), 0.0f); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume
    GImGui->CurrentWindow->DrawList->AddLine(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f}, ImGui::GetColorU32(ImGuiCol_SeparatorHovered)); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume

    ed::Begin("Node editor");

    render_editor(nodes_cfg, library);

    graph_has_changed |= process_creations(nodes_cfg);
    graph_has_changed |= process_deletions(_graph, _frame_nodes, wants_to_delete_selection());

    if (wants_to_open_nodes_menu() || _link_has_just_been_released)
    {
        open_nodes_menu();
        if (!_link_has_just_been_released)
            _pin_to_link_to_new_node = nullptr;
        _link_has_just_been_released = false;
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
        auto const new_node_def_id = imgui_nodes_menu(library, nodes_cfg.maybe_disable_node_definition(), _menu_just_opened);
        _menu_just_opened          = false;
        if (ImGui::Selectable("Frame (Comment)"))
        {
            auto const& frame_node = _frame_nodes.emplace_back();
            ed::SetNodePosition(as_ed_id(frame_node.id), _next_node_position);
        }

        if (new_node_def_id)
        {
            ImGui::CloseCurrentPopup();

            auto const new_node_id = _graph.add_node(nodes_cfg.make_node(*new_node_def_id));
            auto*      new_node    = _graph.nodes().get_mutable_ref(new_node_id);
            if (new_node)
            {
                auto const new_node_id_ed = as_ed_id(new_node_id);
                ed::SetNodePosition(new_node_id_ed, _next_node_position);
                ed::SelectNode(new_node_id_ed);

                if (auto const* begin_pin = _pin_to_link_to_new_node)
                {
                    if (begin_pin->kind() == PinKind::Input && !new_node->output_pins().empty())
                    {
                        _graph.add_link(Link{new_node->output_pins()[0].id(), begin_pin->id()});
                    }
                    else if (!new_node->input_pins().empty())
                    {
                        _graph.add_link(Link{begin_pin->id(), new_node->input_pins()[0].id()});
                    }
                }

                nodes_cfg.on_node_created(*new_node, new_node_id, _pin_to_link_to_new_node); // Must be called last, once the node has been fully set up (link created etc.).
            }
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
        else // Frame node
        {
            for (auto& frame_node : _frame_nodes)
            {
                if (ed::NodeId{as_ed_id(frame_node.id)} != _id_of_node_whose_context_menu_is_open)
                    continue;
                if (ImGui::InputText("##frame_title", &frame_node.name, ImGuiInputTextFlags_EnterReturnsTrue))
                    ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndPopup();
    }
    ed::Resume();
    ed::End();

    return graph_has_changed;
}

void NodesEditorImpl::for_each_selected_node(std::function<void(Node const&)> const& callback) const
{
    ed::SetCurrentEditor(_context);
    auto const selected_nodes_ids = get_selected_nodes_ids();
    for (auto const& ed_node_id : selected_nodes_ids)
    {
        auto const node_id = as_reg_id(ed_node_id, _graph);

        auto const* node = _graph.nodes().get_ref(node_id);
        if (node)
        {
            callback(*node);
        }
        else // Frame node
        {
        }
    }
}

auto NodesEditorImpl::is_empty() const -> bool
{
    return _graph.nodes().is_empty() && _frame_nodes.empty();
}

} // namespace Cool