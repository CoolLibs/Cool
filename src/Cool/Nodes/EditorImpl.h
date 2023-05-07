#pragma once
#include <imgui-node-editor/imgui_node_editor.h>
#include <cereal/types/polymorphic.hpp>
#include "Graph.h"
#include "IEditor.h"
#include "Node.h"
#include "NodeId.h"
#include "NodesConfig.h"
#include "NodesLibrary.h"
#include "UniqueImNodeContext.h"
#include "utilities/builders.h"
#include "utilities/widgets.h"

namespace ed   = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

// enum class PinType {
//     Flow,
//     Bool,
//     Int,
//     Float,
//     String,
//     Object,
//     Function,
//     Delegate,
// };

enum class NodeType {
    Blueprint,
    Comment,
};

struct NodeEX;

struct NodeEX {
    ed::NodeId  ID;
    std::string Name;
    // std::vector<PinEX> Inputs;
    // std::vector<PinEX> Outputs;
    ImColor  Color;
    NodeType Type;
    ImVec2   Size;

    std::string State;
    std::string SavedState;

    NodeEX(int id, const char* name, ImColor color = ImColor(255, 255, 255))
        : ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
    {
    }
};

struct LinkEX {
    ed::LinkId ID;

    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;

    LinkEX(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
        : ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {
    }
};

struct NodeIdLess {
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

namespace Cool {

namespace internal {
struct SearchBarState {
public:
    void               on_nodes_menu_open();
    [[nodiscard]] auto get_nodes_filter() const -> std::string const&;
    /// Returns true iff we should select the first node.
    auto imgui_widget() -> bool;

private:
    bool        _should_be_focused = true;
    std::string _nodes_filter{};
};
} // namespace internal

class NodesEditorImpl : public INodesEditor {
public:
    NodesEditorImpl()
    {
        OnStart();
    }
    ~NodesEditorImpl()
    {
        // OnStop(); TODO(JF)
    }
    auto imgui_window(NodesConfig const&, NodesLibrary const&) -> bool override;

    auto graph() const -> Graph const& override { return _graph; }
    auto graph() -> Graph& override { return _graph; }

private:
    auto               handle_link_creation() -> bool;
    auto               handle_link_deletion() -> bool;
    auto               handle_node_deletion() -> bool;
    [[nodiscard]] auto wants_to_delete_selection() const -> bool;

private:
    /// Returns the id of the new node, or a null id if no node was created.
    auto imgui_nodes_menu(NodesConfig const&, NodesLibrary const&, bool just_opened) -> NodeId;
    auto wants_to_open_nodes_menu() const -> bool;
    void open_nodes_menu();

    ImVec2 _next_node_position = {0.f, 0.f};

private:
    // internal::UniqueImNodeContext _context;
    Graph                    _graph;
    bool                     _window_is_hovered = true;
    internal::SearchBarState _search_bar{};

    // EXAMPLE
private:
    int GetNextId();

    ed::LinkId GetNextLinkId();

    NodeEX* FindNode(ed::NodeId id);

    auto FindPin(ed::PinId const&) -> Pin const*;

    auto is_allowed_connection(Pin const&, Pin const&) -> bool;

    // NodeEX* SpawnComment();

    void BuildNodes();

    void OnStart();

    void OnStop();

    // ImColor GetIconColor(PinType type);

    void DrawPinIcon(Pin const&, bool connected, float alpha);

    void render_blueprint_node(Node&, NodeId const&, NodesCategory const*, util::BlueprintNodeBuilder& builder);

    void render_comment_node(NodeEX& node);

    void render_new_link();

    void render_new_node();

    void handle_creations();

    void handle_deletions();

    void render_editor(NodesLibrary const& library);

    void OnFrame(NodesConfig const&, NodesLibrary const&);

    bool _menu_just_opened{false};

    int                 m_NextId = 1;
    std::vector<NodeEX> m_Nodes;

    ed::NodeId contextNodeId  = 0;
    ed::LinkId contextLinkId  = 0;
    ed::PinId  contextPinId   = 0;
    Pin const* newNodeLinkPin = nullptr;
    Pin const* newLinkPin     = nullptr;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Graph", _graph)
            // cereal::make_nvp("Editor State", std::string{ImNodes::SaveEditorStateToIniString(_context->EditorCtx)})
        );
    }

    template<class Archive>
    void load(Archive& archive)
    {
        // std::string editor_state;
        archive(
            _graph
            // editor_state
        );
        // ImNodes::LoadEditorStateFromIniString(_context->EditorCtx, editor_state.c_str(), editor_state.size());
    }
};

} // namespace Cool

#include <cereal/archives/json.hpp>
CEREAL_REGISTER_TYPE(Cool::NodesEditorImpl);                                     /*NOLINT*/
CEREAL_REGISTER_POLYMORPHIC_RELATION(Cool::INodesEditor, Cool::NodesEditorImpl); /*NOLINT*/