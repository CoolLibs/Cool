#pragma once
#include <cereal/types/polymorphic.hpp>
#include "Cool/Nodes/UniqueEdContext.h"
#include "Cool/Path/Path.h"
#include "Graph.h"
#include "IEditor.h"
#include "Node.h"
#include "NodeId.h"
#include "NodesConfig.h"
#include "NodesLibrary.h"
#include "ed.h"
#include "utilities/builders.h"
#include "utilities/widgets.h"

namespace util = ax::NodeEditor::Utilities;

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

namespace Cool {

namespace internal {
struct SearchBarState {
public:
    void               on_nodes_menu_open();
    [[nodiscard]] auto get_nodes_filter() const -> std::string const& { return _nodes_filter; }

    /// Returns true iff we should select the first node.
    auto imgui_widget() -> bool;

private:
    bool        _should_be_focused{};
    std::string _nodes_filter{""};
};
} // namespace internal

class NodesEditorImpl : public INodesEditor {
public:
    auto imgui_windows(NodesConfig&, NodesLibrary const&) -> bool override;

    auto graph() const -> Graph const& override { return _graph; }
    auto graph() -> Graph& override { return _graph; }

private:
    auto imgui_window_workspace(NodesConfig&, NodesLibrary const&) -> bool;
    auto imgui_window_inspector(NodesConfig&, NodesLibrary const&) -> bool;

private:
    /// Returns the id of the node definition that we should create, or std::nullopt if no node should be created.
    auto imgui_nodes_menu(NodesLibrary const&, bool just_opened) -> std::optional<NodeDefinitionAndCategoryName>;
    auto wants_to_open_nodes_menu() const -> bool;
    void open_nodes_menu();

private:
    internal::UniqueEdContext _context{Cool::Path::root() / "cache/nodes-editor.json"};
    Graph                     _graph;
    bool                      _workspace_is_hovered{};
    internal::SearchBarState  _search_bar{};
    ImVec2                    _next_node_position = {0.f, 0.f};
    ed::NodeId                _id_of_node_whose_context_menu_is_open{};
    bool                      _menu_just_opened{false};
    Pin const*                _new_node_link_pin = nullptr;
    Pin const*                _new_link_pin      = nullptr;

    // EXAMPLE
private:
    // NodeEX* SpawnComment();

    void BuildNodes();

    // ImColor GetIconColor(PinType type);

    void render_blueprint_node(Node&, NodeId const&, NodesCategory const*, NodesConfig&, util::BlueprintNodeBuilder& builder);

    // void render_comment_node(NodeEX& node);

    auto process_creations() -> bool;
    // auto process_node_creation() -> bool;
    auto process_link_creation() -> bool;

    void process_link_released();

    void render_editor(NodesLibrary const& library, NodesConfig&);

    auto imgui_workspace(NodesConfig&, NodesLibrary const&) -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Graph", _graph)
        );
    }
};

} // namespace Cool

#include <cereal/archives/json.hpp>
CEREAL_REGISTER_TYPE(Cool::NodesEditorImpl);                                     /*NOLINT*/
CEREAL_REGISTER_POLYMORPHIC_RELATION(Cool::INodesEditor, Cool::NodesEditorImpl); /*NOLINT*/