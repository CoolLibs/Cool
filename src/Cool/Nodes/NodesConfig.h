#pragma once
#include "Cool/Nodes/Graph.h"
#include "Cool/Nodes/NodesLibrary.h"

namespace Cool {

template<typename T>
concept NodesConfig_Concept = requires(T const const_cfg, Node& node, NodeId const& id, NodeDefinition const& node_def, Graph& graph, Cool::NodeDefinitionAndCategoryName const& def_and_cat) { // clang-format off
    const_cfg.imgui_node_body(node, id);
    const_cfg.update_node_with_new_definition(node, node_def, graph);
    { const_cfg.name(node) } -> std::convertible_to<std::string>;
    const_cfg.widget_to_rename_node(node);
    { const_cfg.make_node(def_and_cat) } -> std::convertible_to<Node>;
}; // clang-format on

class NodesConfig {
public:
    void               imgui_node_body(Node& node, NodeId const& id) const { _pimpl->imgui_node_body(node, id); }
    void               update_node_with_new_definition(Node& node, NodeDefinition const& node_def, Graph& graph) const { _pimpl->update_node_with_new_definition(node, node_def, graph); }
    [[nodiscard]] auto name(Node const& node) const -> std::string { return _pimpl->name(node); }
    void               widget_to_rename_node(Node& node) const { _pimpl->widget_to_rename_node(node); }
    [[nodiscard]] auto make_node(Cool::NodeDefinitionAndCategoryName const& def_and_cat) const -> Node { return _pimpl->make_node(def_and_cat); }

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

        virtual void               imgui_node_body(Node&, Cool::NodeId const&) const                                             = 0;
        virtual void               update_node_with_new_definition(Cool::Node&, Cool::NodeDefinition const&, Cool::Graph&) const = 0;
        [[nodiscard]] virtual auto name(Node const&) const -> std::string                                                        = 0;
        virtual void               widget_to_rename_node(Node&) const                                                            = 0;
        [[nodiscard]] virtual auto make_node(Cool::NodeDefinitionAndCategoryName const&) const -> Node                           = 0;
    };

    template<NodesConfig_Concept NodesCfgT>
    struct Model : public Concept {
        explicit Model(NodesCfgT cfg)
            : _cfg{std::move(cfg)}
        {}

        void imgui_node_body(Node& node, Cool::NodeId const& id) const override
        {
            _cfg.imgui_node_body(node, id);
        }
        void update_node_with_new_definition(Cool::Node& node, Cool::NodeDefinition const& node_def, Cool::Graph& graph) const override
        {
            _cfg.update_node_with_new_definition(node, node_def, graph);
        }
        [[nodiscard]] auto name(Node const& node) const -> std::string override
        {
            return _cfg.name(node);
        }
        void widget_to_rename_node(Node& node) const override
        {
            return _cfg.widget_to_rename_node(node);
        }
        [[nodiscard]] auto make_node(Cool::NodeDefinitionAndCategoryName const& def_and_cat) const -> Node override
        {
            return _cfg.make_node(def_and_cat);
        }

        NodesCfgT _cfg;
    };

private:
    std::unique_ptr<Concept> _pimpl;
};

} // namespace Cool