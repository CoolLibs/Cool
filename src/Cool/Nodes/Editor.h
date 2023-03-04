#pragma once

#include <imnodes/imnodes_internal.h>
#include "Graph.h"
#include "NodeId.h"
#include "NodesCfg_Concept.h"
#include "NodesLibrary.h"
#include "UniqueImNodeContext.h"

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

template<NodesCfg_Concept NodesCfg>
class NodesEditor {
public:
    auto imgui_window(NodesCfg const&, NodesLibrary<typename NodesCfg::NodeDefinitionT> const&) -> bool;
    auto add_node(typename NodesCfg::NodeT const& node) -> NodeId { return _graph.add_node(node); }

    auto graph() const -> auto const& { return _graph; }
    auto graph() -> auto& { return _graph; }

private:
    auto               handle_link_creation() -> bool;
    auto               handle_link_deletion() -> bool;
    auto               handle_node_deletion() -> bool;
    [[nodiscard]] auto wants_to_delete_selection() const -> bool;

private:
    /* Nodes Library */
    auto draw_nodes_library_menu_ifn(NodesCfg const&, NodesLibrary<typename NodesCfg::NodeDefinitionT> const&) -> bool;
    auto imgui_nodes_menu(NodesCfg const&, NodesLibrary<typename NodesCfg::NodeDefinitionT> const&, bool just_opened) -> bool;
    auto wants_to_open_nodes_menu() -> bool;
    void open_nodes_menu();

    ImVec2 _next_node_position = {0.f, 0.f};

private:
    internal::UniqueImNodeContext   _context;
    Graph<typename NodesCfg::NodeT> _graph;
    bool                            _window_is_hovered = true;
    internal::SearchBarState        _search_bar{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        archive(
            cereal::make_nvp("Graph", _graph),
            cereal::make_nvp("Editor State", std::string{ImNodes::SaveEditorStateToIniString(_context->EditorCtx)})
        );
    }

    template<class Archive>
    void load(Archive& archive)
    {
        std::string editor_state;
        archive(
            _graph,
            editor_state
        );
        ImNodes::SetCurrentContext(&*_context);
        ImNodes::LoadEditorStateFromIniString(_context->EditorCtx, editor_state.c_str(), editor_state.size());
    }
};

} // namespace Cool

#include "Editor.tpp"