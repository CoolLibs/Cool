#pragma once
#include "imgui-node-editor/imgui_node_editor.h"
#include "utilities/builders.h"
#include "utilities/widgets.h"

namespace ed   = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

enum class PinType {
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

enum class PinKind {
    Output,
    Input
};

enum class NodeType {
    Blueprint,
    Comment,
};

struct Node;

struct Pin {
    ed::PinId   ID;
    ::Node*     Node;
    std::string Name;
    PinType     Type;
    PinKind     Kind;

    Pin(int id, const char* name, PinType type)
        : ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
    {
    }
};

struct Node {
    ed::NodeId       ID;
    std::string      Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor          Color;
    NodeType         Type;
    ImVec2           Size;

    std::string State;
    std::string SavedState;

    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255))
        : ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
    {
    }
};

struct Link {
    ed::LinkId ID;

    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;

    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
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

struct Example {
    int GetNextId();

    ed::LinkId GetNextLinkId();

    Node* FindNode(ed::NodeId id);

    Link* FindLink(ed::LinkId id);

    auto FindPin(ed::PinId id) -> Pin*;

    bool IsPinLinked(ed::PinId id);

    bool CanCreateLink(Pin* a, Pin* b);

    void BuildNode(Node* node);

    Node* SpawnInputActionNode();

    Node* SpawnBranchNode();

    Node* SpawnDoNNode();

    Node* SpawnOutputActionNode();

    Node* SpawnPrintStringNode();

    Node* SpawnComment();

    void BuildNodes();

    void OnStart();

    void OnStop();

    ImColor GetIconColor(PinType type);

    void DrawPinIcon(const Pin& pin, bool connected, int alpha);

    void render_blueprint_node(Node& node, util::BlueprintNodeBuilder& builder);

    void render_comment_node(Node& node);

    void render_new_link();

    void render_new_node();

    void handle_creations();

    void handle_deletions();

    void render_editor();

    auto nodes_menu() -> Node*;

    void OnFrame();

    int               m_NextId      = 1;
    const int         m_PinIconSize = 24;
    std::vector<Node> m_Nodes;
    std::vector<Link> m_Links;

    ed::NodeId contextNodeId  = 0;
    ed::LinkId contextLinkId  = 0;
    ed::PinId  contextPinId   = 0;
    bool       createNewNode  = false;
    Pin*       newNodeLinkPin = nullptr;
    Pin*       newLinkPin     = nullptr;

    float leftPaneWidth  = 000.0f;
    float rightPaneWidth = 800.0f;
};
