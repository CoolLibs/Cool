
#include "Cool/Nodes/EditorImpl.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include <Cool/ImGui/icon_fmt.h>
#include <imgui.h>
#include <imgui/imgui_internal.h>
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/ImGui/IcoMoonCodepoints.h"
#include "EditorImpl.h"

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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.f, 0.f});
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
    ImGui::Begin(icon_fmt("Nodes", ICOMOON_TREE).c_str(), nullptr, ImGuiWindowFlags_NoScrollbar);
    ImGui::PopStyleVar(2);
    auto const prev_tesselation                  = ImGui::GetStyle().CircleTessellationMaxError;
    ImGui::GetStyle().CircleTessellationMaxError = 0.1f; // Make borders smooth even when zooming.
    OnFrame(nodes_cfg, library);
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

static ed::EditorContext* m_Editor = nullptr;

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

int NodesEditorImpl::GetNextId()
{
    return m_NextId++;
}

ed::LinkId NodesEditorImpl::GetNextLinkId()
{
    return ed::LinkId(GetNextId());
}

NodeEX* NodesEditorImpl::FindNode(ed::NodeId id)
{
    for (auto& node : m_Nodes)
        if (node.ID == id)
            return &node;

    return nullptr;
}

LinkEX* NodesEditorImpl::FindLink(ed::LinkId id)
{
    for (auto& link : m_Links)
        if (link.ID == id)
            return &link;

    return nullptr;
}

auto NodesEditorImpl::FindPin(ed::PinId id) -> PinEX*
{
    if (!id)
        return nullptr;

    for (auto& node : m_Nodes)
    {
        for (auto& pin : node.Inputs)
            if (pin.ID == id)
                return &pin;

        for (auto& pin : node.Outputs)
            if (pin.ID == id)
                return &pin;
    }

    return nullptr;
}

bool NodesEditorImpl::IsPinLinked(ed::PinId id)
{
    if (!id)
        return false;

    for (auto& link : m_Links)
        if (link.StartPinID == id || link.EndPinID == id)
            return true;

    return false;
}

bool NodesEditorImpl::CanCreateLink(PinEX* a, PinEX* b)
{
    if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
        return false;

    return true;
}

void NodesEditorImpl::BuildNode(NodeEX* node)
{
    for (auto& input : node->Inputs)
    {
        input.Node = node;
        input.Kind = PinKind::Input;
    }

    for (auto& output : node->Outputs)
    {
        output.Node = node;
        output.Kind = PinKind::Output;
    }
}

NodeEX* NodesEditorImpl::SpawnInputActionNode()
{
    m_Nodes.emplace_back(GetNextId(), "InputAction Fire", ImColor(255, 128, 128));
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Delegate);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "Pressed", PinType::Flow);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "Released", PinType::Flow);

    BuildNode(&m_Nodes.back());

    return &m_Nodes.back();
}

NodeEX* NodesEditorImpl::SpawnBranchNode()
{
    m_Nodes.emplace_back(GetNextId(), "Branch");
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "True", PinType::Flow);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "False", PinType::Flow);

    BuildNode(&m_Nodes.back());

    return &m_Nodes.back();
}

NodeEX* NodesEditorImpl::SpawnDoNNode()
{
    m_Nodes.emplace_back(GetNextId(), "Do N");
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "Enter", PinType::Flow);
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "N", PinType::Int);
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "Reset", PinType::Flow);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "Exit", PinType::Flow);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "Counter", PinType::Int);

    BuildNode(&m_Nodes.back());

    return &m_Nodes.back();
}

NodeEX* NodesEditorImpl::SpawnOutputActionNode()
{
    m_Nodes.emplace_back(GetNextId(), "OutputAction");
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "Sample", PinType::Float);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "Event", PinType::Delegate);

    BuildNode(&m_Nodes.back());

    return &m_Nodes.back();
}

NodeEX* NodesEditorImpl::SpawnPrintStringNode()
{
    m_Nodes.emplace_back(GetNextId(), "Print String");
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
    m_Nodes.back().Inputs.emplace_back(GetNextId(), "In String", PinType::String);
    m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

    BuildNode(&m_Nodes.back());

    return &m_Nodes.back();
}

NodeEX* NodesEditorImpl::SpawnComment()
{
    m_Nodes.emplace_back(GetNextId(), "Test Comment");
    m_Nodes.back().Type = NodeType::Comment;
    m_Nodes.back().Size = ImVec2(300, 200);

    return &m_Nodes.back();
}

void NodesEditorImpl::BuildNodes()
{
    for (auto& node : m_Nodes)
        BuildNode(&node);
}

void NodesEditorImpl::OnStart()
{
    ed::Config config;

    config.SettingsFile = "Blueprints.json";

    config.UserPointer = this;

    config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t {
        auto self = static_cast<NodesEditorImpl*>(userPointer);

        auto node = self->FindNode(nodeId);
        if (!node)
            return 0;

        if (data != nullptr)
            memcpy(data, node->State.data(), node->State.size());
        return node->State.size();
    };

    config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason, void* userPointer) -> bool {
        auto self = static_cast<NodesEditorImpl*>(userPointer);

        auto node = self->FindNode(nodeId);
        if (!node)
            return false;

        node->State.assign(data, size);

        return true;
    };

    m_Editor = ed::CreateEditor(&config);
    ed::SetCurrentEditor(m_Editor);

    NodeEX* node;
    node = SpawnInputActionNode();
    ed::SetNodePosition(node->ID, ImVec2(-252, 220));
    node = SpawnBranchNode();
    ed::SetNodePosition(node->ID, ImVec2(-300, 351));
    node = SpawnDoNNode();
    ed::SetNodePosition(node->ID, ImVec2(-238, 504));
    node = SpawnOutputActionNode();
    ed::SetNodePosition(node->ID, ImVec2(71, 80));

    node = SpawnComment();
    ed::SetNodePosition(node->ID, ImVec2(112, 576));
    ed::SetGroupSize(node->ID, ImVec2(384, 154));
    node = SpawnComment();
    ed::SetNodePosition(node->ID, ImVec2(800, 224));
    ed::SetGroupSize(node->ID, ImVec2(640, 400));

    ed::NavigateToContent();

    BuildNodes();
}

void NodesEditorImpl::OnStop()
{
    if (m_Editor)
    {
        ed::DestroyEditor(m_Editor);
        m_Editor = nullptr;
    }
}

ImColor NodesEditorImpl::GetIconColor(PinType type)
{
    switch (type)
    {
    default:
    case PinType::Flow: return ImColor(255, 255, 255);
    case PinType::Bool: return ImColor(220, 48, 48);
    case PinType::Int: return ImColor(68, 201, 156);
    case PinType::Float: return ImColor(147, 226, 74);
    case PinType::String: return ImColor(124, 21, 153);
    case PinType::Object: return ImColor(51, 150, 215);
    case PinType::Function: return ImColor(218, 0, 183);
    case PinType::Delegate: return ImColor(255, 48, 48);
    }
};

void NodesEditorImpl::DrawPinIcon(const PinEX& pin, bool connected, int alpha)
{
    IconType iconType;
    ImColor  color = GetIconColor(pin.Type);
    color.Value.w  = alpha / 255.0f;
    switch (pin.Type)
    {
    case PinType::Flow: iconType = IconType::Flow; break;
    case PinType::Bool: iconType = IconType::Circle; break;
    case PinType::Int: iconType = IconType::Circle; break;
    case PinType::Float: iconType = IconType::Circle; break;
    case PinType::String: iconType = IconType::Circle; break;
    case PinType::Object: iconType = IconType::Circle; break;
    case PinType::Function: iconType = IconType::Circle; break;
    case PinType::Delegate: iconType = IconType::Square; break;
    default:
        return;
    }

    ax::Widgets::Icon(ImVec2(24.f, 24.f), iconType, connected, color, ImColor(32, 32, 32, alpha));
};

void NodesEditorImpl::render_blueprint_node(NodeEX& node, util::BlueprintNodeBuilder& builder)
{
    bool hasOutputDelegates = false;
    for (auto& output : node.Outputs)
        if (output.Type == PinType::Delegate)
            hasOutputDelegates = true;

    builder.Begin(node.ID);

    builder.Header(node.Color);
    ImGui::Spring(0);
    ImGui::TextUnformatted(node.Name.c_str());
    ImGui::Spring(1);
    ImGui::Dummy(ImVec2(0, 28));
    if (hasOutputDelegates)
    {
        ImGui::BeginVertical("delegates", ImVec2(0, 28));
        ImGui::Spring(1, 0);
        for (auto& output : node.Outputs)
        {
            if (output.Type != PinType::Delegate)
                continue;

            auto alpha = ImGui::GetStyle().Alpha;
            if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                alpha = alpha * (48.0f / 255.0f);

            ed::BeginPin(output.ID, ed::PinKind::Output);
            ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
            ed::PinPivotSize(ImVec2(0, 0));
            ImGui::BeginHorizontal(output.ID.AsPointer());
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
            if (!output.Name.empty())
            {
                ImGui::TextUnformatted(output.Name.c_str());
                ImGui::Spring(0);
            }
            DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
            ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
            ImGui::EndHorizontal();
            ImGui::PopStyleVar();
            ed::EndPin();

            // DrawItemRect(ImColor(255, 0, 0));
        }
        ImGui::Spring(1, 0);
        ImGui::EndVertical();
        ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
    }
    else
        ImGui::Spring(0);
    builder.EndHeader();

    for (auto& input : node.Inputs)
    {
        auto alpha = ImGui::GetStyle().Alpha;
        if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
            alpha = alpha * (48.0f / 255.0f);

        builder.Input(input.ID);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
        ImGui::Spring(0);
        if (!input.Name.empty())
        {
            ImGui::TextUnformatted(input.Name.c_str());
            ImGui::Spring(0);
        }
        if (input.Type == PinType::Bool)
        {
            ImGui::Button("Hello");
            ImGui::Spring(0);
        }
        ImGui::PopStyleVar();
        builder.EndInput();
    }

    for (auto& output : node.Outputs)
    {
        if (output.Type == PinType::Delegate)
            continue;

        auto alpha = ImGui::GetStyle().Alpha;
        if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
            alpha = alpha * (48.0f / 255.0f);

        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
        builder.Output(output.ID);
        if (output.Type == PinType::String)
        {
            static char buffer[128] = "Edit Me\nMultiline!";
            static bool wasActive   = false;

            ImGui::PushItemWidth(100.0f);
            ImGui::InputText("##edit", buffer, 127);
            ImGui::PopItemWidth();
            if (ImGui::IsItemActive() && !wasActive)
            {
                ed::EnableShortcuts(false);
                wasActive = true;
            }
            else if (!ImGui::IsItemActive() && wasActive)
            {
                ed::EnableShortcuts(true);
                wasActive = false;
            }
            ImGui::Spring(0);
        }
        if (!output.Name.empty())
        {
            ImGui::Spring(0);
            ImGui::TextUnformatted(output.Name.c_str());
        }
        ImGui::Spring(0);
        DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
        ImGui::PopStyleVar();
        builder.EndOutput();
    }

    builder.End();
}

void NodesEditorImpl::render_comment_node(NodeEX& node)
{
    const float commentAlpha = 0.75f;

    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
    ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
    ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
    ed::BeginNode(node.ID);
    ImGui::PushID(node.ID.AsPointer());
    ImGui::BeginVertical("content");
    ImGui::BeginHorizontal("horizontal");
    ImGui::Spring(1);
    ImGui::TextUnformatted(node.Name.c_str());
    ImGui::Spring(1);
    ImGui::EndHorizontal();
    ed::Group(node.Size);
    ImGui::EndVertical();
    ImGui::PopID();
    ed::EndNode();
    ed::PopStyleColor(2);
    ImGui::PopStyleVar();

    if (ed::BeginGroupHint(node.ID))
    {
        // auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
        auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

        // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

        auto min = ed::GetGroupMin();
        // auto max = ed::GetGroupMax();

        ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
        ImGui::BeginGroup();
        ImGui::TextUnformatted(node.Name.c_str());
        ImGui::EndGroup();

        auto drawList = ed::GetHintBackgroundDrawList();

        auto hintBounds      = ImGui_GetItemRect();
        auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

        drawList->AddRectFilled(
            hintFrameBounds.GetTL(),
            hintFrameBounds.GetBR(),
            IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f
        );

        drawList->AddRect(
            hintFrameBounds.GetTL(),
            hintFrameBounds.GetBR(),
            IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f
        );

        // ImGui::PopStyleVar();
    }
    ed::EndGroupHint();
}

void NodesEditorImpl::render_new_link()
{
    ed::PinId startPinId = 0, endPinId = 0;
    if (ed::QueryNewLink(&startPinId, &endPinId))
    {
        auto startPin = FindPin(startPinId);
        auto endPin   = FindPin(endPinId);

        newLinkPin = startPin ? startPin : endPin;

        if (startPin->Kind == PinKind::Input)
        {
            std::swap(startPin, endPin);
            std::swap(startPinId, endPinId);
        }

        if (startPin && endPin)
        {
            if (endPin == startPin)
            {
                ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
            }
            else if (endPin->Kind == startPin->Kind)
            {
                show_label("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
            }
            // else if (endPin->Node == startPin->Node)
            //{
            //     show_label("x Cannot connect to self", ImColor(45, 32, 32, 180));
            //     ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
            // }
            else if (endPin->Type != startPin->Type)
            {
                show_label("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
            }
            else
            {
                show_label("+ Create Link", ImColor(32, 45, 32, 180));
                if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                {
                    m_Links.emplace_back(LinkEX(GetNextId(), startPinId, endPinId));
                    m_Links.back().Color = GetIconColor(startPin->Type);
                }
            }
        }
    }
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
            createNewNode  = true;
            newNodeLinkPin = FindPin(pinId);
            newLinkPin     = nullptr;
            ed::Suspend();
            ImGui::OpenPopup("Create New Node");
            ed::Resume();
        }
    }
}

void NodesEditorImpl::handle_creations()
{
    if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
    {
        render_new_link();
        render_new_node();
    }
    else
        newLinkPin = nullptr;

    ed::EndCreate();
}

void NodesEditorImpl::handle_deletions()
{
    auto scope_guard = sg::make_scope_guard([] { ed::EndDelete(); });
    if (!ed::BeginDelete())
        return;

    ed::LinkId linkId = 0;
    while (ed::QueryDeletedLink(&linkId))
    {
        if (ed::AcceptDeletedItem())
        {
            auto id = std::find_if(m_Links.begin(), m_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
            if (id != m_Links.end())
                m_Links.erase(id);
        }
    }

    ed::NodeId nodeId = 0;
    while (ed::QueryDeletedNode(&nodeId))
    {
        if (!ed::AcceptDeletedItem())
            continue;
        auto id = std::find_if(m_Nodes.begin(), m_Nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
        if (id != m_Nodes.end())
            m_Nodes.erase(id);
    }
}

void NodesEditorImpl::render_editor()
{
    // auto cursorTopLeft = ImGui::GetCursorScreenPos();

    util::BlueprintNodeBuilder builder{};

    for (auto& node : m_Nodes)
    {
        if (node.Type == NodeType::Blueprint)
            render_blueprint_node(node, builder);
        if (node.Type == NodeType::Comment)
            render_comment_node(node);
    }

    for (auto& link : m_Links)
        ed::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);

    if (!createNewNode)
    {
        handle_creations();
        handle_deletions();
    }

    // ImGui::SetCursorScreenPos(cursorTopLeft);
}

void NodesEditorImpl::OnFrame(NodesConfig const& nodes_cfg, NodesLibrary const& library)
{
    ed::SetCurrentEditor(m_Editor);
    // GImGui->CurrentWindow->DrawList->AddRectFilled(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f},  ImGui::GetColorU32(ImGuiCol_SeparatorHovered), 0.0f); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume
    GImGui->CurrentWindow->DrawList->AddLine(ImVec2{0.f, 0.f}, ImVec2{0.f, 0.f}, ImGui::GetColorU32(ImGuiCol_SeparatorHovered)); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming in) EDIT this is caused by the suspend / resume

    ed::Begin("Node editor");
    {
        render_editor();
    }

    auto openPopupPosition = ImGui::GetMousePos();
    ed::Suspend();
    if (ed::ShowBackgroundContextMenu())
    {
        ImGui::OpenPopup("Create New Node");
        newNodeLinkPin = nullptr;
    }

    if (ImGui::BeginPopup("Create New Node"))
    {
        bool has_created_node = imgui_nodes_menu(nodes_cfg, library, false);

        // if (node)
        // {
        //     createNewNode = false;

        //     ed::SetNodePosition(node->ID, openPopupPosition);

        //     if (auto startPin = newNodeLinkPin)
        //     {
        //         auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

        //         for (auto& pin : pins)
        //         {
        //             if (CanCreateLink(startPin, &pin))
        //             {
        //                 auto endPin = &pin;
        //                 if (startPin->Kind == PinKind::Input)
        //                     std::swap(startPin, endPin);

        //                 m_Links.emplace_back(LinkEX(GetNextId(), startPin->ID, endPin->ID));
        //                 m_Links.back().Color = GetIconColor(startPin->Type);

        //                 break;
        //             }
        //         }
        //     }
        // }

        ImGui::EndPopup();
    }
    else
        createNewNode = false;
    ed::Resume();
    ed::End();
}

} // namespace Cool