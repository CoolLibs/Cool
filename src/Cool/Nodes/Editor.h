#pragma once

#include <Cool/Dependencies/Dirty.h>
#include <imnodes/imnodes_internal.h>
#include "Graph.h"
#include "NodeC.h"
#include "UniqueImNodeContext.h"

namespace Cool::Nodes {

// template<NodeC Node> // TODO(JF)
class Editor {
public:
    explicit Editor(std::string_view nodes_folder_path);
    void imgui_window(SetDirty_Ref set_dirty);
    // void update_templates_and_nodes();
    // void ask_to_open_nodes_menu();
    // bool tree_has_changed();
    // bool tree_is_valid() const { return _all_nodes_have_a_valid_template; }
    // auto tree() const -> const NodeGraph& { return _graph; }
    void add_node(Node const& node) { _graph.add_node(node); }
    // void remove_all_nodes()
    // {
    //     _graph.remove_all_nodes();
    //     _graph_has_changed = true;
    // }

    // bool wants_to_delete_selection() const;
    // void on_graph_change();
    // bool handle_link_creation();
    // bool handle_link_deletion();
    // bool handle_node_deletion();

private: /* Nodes Library */
    void show_nodes_library_menu_ifn(SetDirty_Ref set_dirty);
    bool imgui_nodes_menu();
    auto wants_to_open_nodes_menu() -> bool;
    void open_nodes_menu();

    ImVec2 _next_node_position = {0.f, 0.f};

private:
    internal::UniqueImNodeContext _context;
    // NodeFactory                 _factory;
    Graph     _graph;
    DirtyFlag _graph_dirty_flag;
    // bool      _all_nodes_have_a_valid_template = true;
    // bool      _window_is_hovered               = false;

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
        ImNodes::LoadEditorStateFromIniString(_context->EditorCtx, editor_state.c_str(), editor_state.size());
        // update_templates_and_nodes();
    }
};

} // namespace Cool::Nodes