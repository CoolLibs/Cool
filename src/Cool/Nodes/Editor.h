#pragma once

#include "internal/Graph.h"
#include "internal/NodeC.h"
#include "internal/UniqueImNodeContext.h"

namespace Cool::Nodes {

// template<NodeC Node> // TODO(JF)
class Editor {
public:
    explicit Editor(std::string_view nodes_folder_path);
    void imgui_window();
    // void update_templates_and_nodes();
    // void ask_to_open_nodes_menu();
    // bool tree_has_changed();
    // bool tree_is_valid() const { return _all_nodes_have_a_valid_template; }
    // auto tree() const -> const NodeGraph& { return _graph; }
    // void add_node(const Node& node) { _graph.add_node(node); }
    // void remove_all_nodes()
    // {
    //     _graph.remove_all_nodes();
    //     _graph_has_changed = true;
    // }

private:
    // auto should_open_nodes_menu() -> bool;
    // void open_nodes_menu();
    // bool wants_to_delete_selection() const;
    // void on_graph_change();
    // bool imgui_nodes_menu();
    // bool handle_link_creation();
    // bool handle_link_deletion();
    // bool handle_node_deletion();

private:
    internal::UniqueImNodeContext _context;
    // NodeFactory                 _factory;
    Graph _graph;
    // bool      _all_nodes_have_a_valid_template = true;
    // bool      _graph_has_changed               = true;
    // bool      _should_open_nodes_menu          = false;
    // bool      _window_is_hovered               = false;
    // ImVec2    _next_node_position              = {0.f, 0.f};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void save(Archive& archive) const
    {
        ImNodes::SetCurrentContext(&*_context);
        archive(
            cereal::make_nvp("Graph", _graph),
            cereal::make_nvp("Editor State", std::string{ImNodes::SaveCurrentEditorStateToIniString()})
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
        ImNodes::LoadCurrentEditorStateFromIniString(editor_state.c_str(), editor_state.size());
        // update_templates_and_nodes();
    }
};

} // namespace Cool::Nodes