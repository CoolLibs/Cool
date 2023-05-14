#pragma once
#include "Cool/Nodes/Graph.h"
#include "Cool/Nodes/NodesLibrary.h"

namespace Cool {

template<typename T>
concept NodesConfig_Concept = requires(T const const_cfg, T cfg, Node& node, NodeId const& id, NodeDefinition const& node_def, Graph& graph, Cool::NodeDefinitionAndCategoryName const& def_and_cat) { // clang-format off
    cfg.imgui_node_above_pins(node, id);
    cfg.imgui_node_below_pins(node, id);
    cfg.imgui_node_in_inspector(node, id);
    cfg.update_node_with_new_definition(node, node_def, graph);
    { const_cfg.name(node) } -> std::convertible_to<std::string>;
    cfg.widget_to_rename_node(node);
    { cfg.make_node(def_and_cat) } -> std::convertible_to<Node>;
}; // clang-format on

class NodesConfig {
public:
    void               imgui_node_above_pins(Node& node, NodeId const& id) { _pimpl->imgui_node_above_pins(node, id); }
    void               imgui_node_below_pins(Node& node, NodeId const& id) { _pimpl->imgui_node_below_pins(node, id); }
    void               imgui_node_in_inspector(Node& node, NodeId const& id) { _pimpl->imgui_node_in_inspector(node, id); }
    void               update_node_with_new_definition(Node& node, NodeDefinition const& node_def, Graph& graph) { _pimpl->update_node_with_new_definition(node, node_def, graph); }
    [[nodiscard]] auto name(Node const& node) const -> std::string { return _pimpl->name(node); }
    void               widget_to_rename_node(Node& node) { _pimpl->widget_to_rename_node(node); }
    [[nodiscard]] auto make_node(Cool::NodeDefinitionAndCategoryName const& def_and_cat) -> Node { return _pimpl->make_node(def_and_cat); }

public: // Type-erasure implementation details
    template<NodesConfig_Concept NodesCfgT>
    NodesConfig(NodesCfgT cfg) // NOLINT(*-explicit-constructor, *-explicit-conversions) A type-erased object should be implicitly created from objects matching its requirements.
        : _pimpl{std::make_unique<Model<NodesCfgT>>(std::move(cfg))}
    {}

    NodesConfig(NodesConfig const&)                        = delete;
    auto operator=(NodesConfig const&) -> NodesConfig&     = delete;
    NodesConfig(NodesConfig&&) noexcept                    = default;
    auto operator=(NodesConfig&&) noexcept -> NodesConfig& = default;
    ~NodesConfig()                                         = default;

private:
    struct Concept { // NOLINT(*-special-member-functions)
        virtual ~Concept() = default;

        virtual void               imgui_node_above_pins(Node&, Cool::NodeId const&)                                       = 0;
        virtual void               imgui_node_below_pins(Node&, Cool::NodeId const&)                                       = 0;
        virtual void               imgui_node_in_inspector(Node&, Cool::NodeId const&)                                     = 0;
        virtual void               update_node_with_new_definition(Cool::Node&, Cool::NodeDefinition const&, Cool::Graph&) = 0;
        [[nodiscard]] virtual auto name(Node const&) const -> std::string                                                  = 0;
        virtual void               widget_to_rename_node(Node&)                                                            = 0;
        [[nodiscard]] virtual auto make_node(Cool::NodeDefinitionAndCategoryName const&) -> Node                           = 0;
    };

    template<NodesConfig_Concept NodesCfgT>
    struct Model : public Concept {
        explicit Model(NodesCfgT cfg)
            : _cfg{std::move(cfg)}
        {}

        void imgui_node_above_pins(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_node_above_pins(node, id);
        }
        void imgui_node_below_pins(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_node_below_pins(node, id);
        }
        void imgui_node_in_inspector(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_node_in_inspector(node, id);
        }
        void update_node_with_new_definition(Cool::Node& node, Cool::NodeDefinition const& node_def, Cool::Graph& graph) override
        {
            _cfg.update_node_with_new_definition(node, node_def, graph);
        }
        [[nodiscard]] auto name(Node const& node) const -> std::string override
        {
            return _cfg.name(node);
        }
        void widget_to_rename_node(Node& node) override
        {
            return _cfg.widget_to_rename_node(node);
        }
        [[nodiscard]] auto make_node(Cool::NodeDefinitionAndCategoryName const& def_and_cat) -> Node override
        {
            return _cfg.make_node(def_and_cat);
        }

        NodesCfgT _cfg;
    };

private:
    std::unique_ptr<Concept> _pimpl;
};

} // namespace Cool