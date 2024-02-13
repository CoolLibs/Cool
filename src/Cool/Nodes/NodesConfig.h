#pragma once
#include "Cool/Nodes/NodesGraph.h"
#include "Cool/Nodes/NodesLibrary.h"

namespace Cool {

template<typename T>
concept NodesConfig_Concept = requires(T const const_cfg, T cfg, size_t idx, Pin const& pin_const, Node& node, Node const& node_const, NodeId const& node_id, Link const& link_const, LinkId const& link_id, NodeDefinition const& node_def, NodesGraph& graph, Cool::NodeDefinitionAndCategoryName const& def_and_cat, Pin const* pin_linked_to_new_node) { // clang-format off
    cfg.imgui_above_node_pins(node, node_id);
    cfg.imgui_below_node_pins(node, node_id);
    cfg.imgui_in_inspector_above_node_info(node, node_id);
    cfg.imgui_in_inspector_below_node_info(node, node_id);
    { const_cfg.node_color(node_const, node_id) } -> std::convertible_to<Cool::Color>;
    { const_cfg.pin_color(pin_const, idx, node_const, node_id) } -> std::convertible_to<Cool::Color>;
    cfg.on_link_created_between_existing_nodes(link_const, link_id);
    cfg.update_node_with_new_definition(node, node_def, graph);
    { const_cfg.name(node) } -> std::convertible_to<std::string>;
    cfg.widget_to_rename_node(node);
    { cfg.add_node(def_and_cat) } -> std::convertible_to<NodeId>;
    cfg.on_node_created(node, node_id, pin_linked_to_new_node);
    { const_cfg.copy_nodes() } -> std::convertible_to<std::string>;
    { cfg.paste_nodes("Some clipboard content") } -> std::convertible_to<bool>;
    { const_cfg.maybe_disable_node_definition() } -> std::convertible_to<MaybeDisableNodeDefinition>;
}; // clang-format on

class NodesConfig {
public:
    void               imgui_above_node_pins(Node& node, NodeId const& id) { _pimpl->imgui_above_node_pins(node, id); }
    void               imgui_below_node_pins(Node& node, NodeId const& id) { _pimpl->imgui_below_node_pins(node, id); }
    void               imgui_in_inspector_above_node_info(Node& node, NodeId const& id) { _pimpl->imgui_in_inspector_above_node_info(node, id); }
    void               imgui_in_inspector_below_node_info(Node& node, NodeId const& id) { _pimpl->imgui_in_inspector_below_node_info(node, id); }
    [[nodiscard]] auto node_color(Node const& node, NodeId const& id) const -> Cool::Color { return _pimpl->node_color(node, id); }
    [[nodiscard]] auto pin_color(Pin const& pin, size_t pin_index, Node const& node, NodeId const& id) const -> Cool::Color { return _pimpl->pin_color(pin, pin_index, node, id); }
    /// Doesn't get called when a link is released on the workspace and creates a new node (If you want to handle that, you already have on_node_created(): if pin_linked_to_new_node is not null then this means said event occurred).
    void               on_link_created_between_existing_nodes(Link const& link, LinkId const& id) { _pimpl->on_link_created_between_existing_nodes(link, id); }
    void               update_node_with_new_definition(Node& node, NodeDefinition const& node_def, NodesGraph& graph) { _pimpl->update_node_with_new_definition(node, node_def, graph); }
    [[nodiscard]] auto name(Node const& node) const -> std::string { return _pimpl->name(node); }
    void               widget_to_rename_node(Node& node) { _pimpl->widget_to_rename_node(node); }
    [[nodiscard]] auto add_node(Cool::NodeDefinitionAndCategoryName const& def_and_cat) -> NodeId { return _pimpl->add_node(def_and_cat); }
    void               on_node_created(Node& node, NodeId const& id, Pin const* pin_linked_to_new_node) { _pimpl->on_node_created(node, id, pin_linked_to_new_node); }
    [[nodiscard]] auto copy_nodes() const -> std::string { return _pimpl->copy_nodes(); }
    /// Returns true iff successfully pasted nodes
    auto paste_nodes(std::string_view clipboard_content) -> bool { return _pimpl->paste_nodes(clipboard_content); }
    auto maybe_disable_node_definition() const -> Cool::MaybeDisableNodeDefinition { return _pimpl->maybe_disable_node_definition(); }

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

        virtual void               imgui_above_node_pins(Node&, Cool::NodeId const&)                                              = 0;
        virtual void               imgui_below_node_pins(Node&, Cool::NodeId const&)                                              = 0;
        virtual void               imgui_in_inspector_above_node_info(Node&, Cool::NodeId const&)                                 = 0;
        virtual void               imgui_in_inspector_below_node_info(Node&, Cool::NodeId const&)                                 = 0;
        [[nodiscard]] virtual auto node_color(Node const&, Cool::NodeId const&) const -> Cool::Color                              = 0;
        [[nodiscard]] virtual auto pin_color(Pin const&, size_t pin_index, Node const&, Cool::NodeId const&) const -> Cool::Color = 0;
        virtual void               on_link_created_between_existing_nodes(Link const&, Cool::LinkId const&)                       = 0;
        virtual void               update_node_with_new_definition(Cool::Node&, Cool::NodeDefinition const&, Cool::NodesGraph&)   = 0;
        [[nodiscard]] virtual auto name(Node const&) const -> std::string                                                         = 0;
        virtual void               widget_to_rename_node(Node&)                                                                   = 0;
        [[nodiscard]] virtual auto add_node(Cool::NodeDefinitionAndCategoryName const&) -> NodeId                                 = 0;
        virtual void               on_node_created(Node&, Cool::NodeId const&, Pin const* pin_linked_to_new_node)                 = 0;
        [[nodiscard]] virtual auto copy_nodes() const -> std::string                                                              = 0;
        virtual auto               paste_nodes(std::string_view clipboard_content) -> bool                                        = 0;
        virtual auto               maybe_disable_node_definition() const -> Cool::MaybeDisableNodeDefinition                      = 0;
    };

    template<NodesConfig_Concept NodesCfgT>
    struct Model : public Concept {
        explicit Model(NodesCfgT cfg)
            : _cfg{std::move(cfg)}
        {}

        void imgui_above_node_pins(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_above_node_pins(node, id);
        }
        void imgui_below_node_pins(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_below_node_pins(node, id);
        }
        void imgui_in_inspector_above_node_info(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_in_inspector_above_node_info(node, id);
        }
        void imgui_in_inspector_below_node_info(Node& node, Cool::NodeId const& id) override
        {
            _cfg.imgui_in_inspector_below_node_info(node, id);
        }
        [[nodiscard]] auto node_color(Node const& node, Cool::NodeId const& id) const -> Cool::Color override
        {
            return _cfg.node_color(node, id);
        }
        [[nodiscard]] auto pin_color(Pin const& pin, size_t pin_index, Node const& node, Cool::NodeId const& id) const -> Cool::Color override
        {
            return _cfg.pin_color(pin, pin_index, node, id);
        }
        void on_link_created_between_existing_nodes(Link const& link, Cool::LinkId const& id) override
        {
            _cfg.on_link_created_between_existing_nodes(link, id);
        }
        void update_node_with_new_definition(Cool::Node& node, Cool::NodeDefinition const& node_def, Cool::NodesGraph& graph) override
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
        [[nodiscard]] auto add_node(Cool::NodeDefinitionAndCategoryName const& def_and_cat) -> NodeId override
        {
            return _cfg.add_node(def_and_cat);
        }
        void on_node_created(Node& node, Cool::NodeId const& id, Pin const* pin_linked_to_new_node) override
        {
            _cfg.on_node_created(node, id, pin_linked_to_new_node);
        }
        [[nodiscard]] auto copy_nodes() const -> std::string override
        {
            return _cfg.copy_nodes();
        }
        auto paste_nodes(std::string_view clipboard_content) -> bool override
        {
            return _cfg.paste_nodes(clipboard_content);
        }
        auto maybe_disable_node_definition() const -> Cool::MaybeDisableNodeDefinition override
        {
            return _cfg.maybe_disable_node_definition();
        }

        NodesCfgT _cfg;
    };

private:
    std::unique_ptr<Concept> _pimpl;
};

} // namespace Cool