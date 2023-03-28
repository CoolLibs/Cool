#pragma once

#include <memory>
namespace Cool {

template<typename T>
concept NodeDefinition_Concept = requires(T node_def) { // clang-format off
    { node_def.name() } -> std::convertible_to<std::string>;
}; // clang-format on

class NodeDefinition {
public:
    [[nodiscard]] auto name() const -> std::string { return _pimpl->get_name(); }

    template<NodeDefinition_Concept NodeDefT>
    [[nodiscard]] auto downcast() -> NodeDefT&
    {
        return static_cast<Model<NodeDefT>&>(*_pimpl)._node_def;
    }
    template<NodeDefinition_Concept NodeDefT>
    [[nodiscard]] auto downcast() const -> NodeDefT const&
    {
        return static_cast<Model<NodeDefT> const&>(*_pimpl)._node_def;
    }

public: // Type-erasure implementation details
    NodeDefinition() = default;

    template<NodeDefinition_Concept NodeDefT>
    NodeDefinition(NodeDefT def) // NOLINT(*-explicit-constructor, *-explicit-conversions) A type-erased object should be implicitly created from objects matching its requirements.
        : _pimpl{std::make_unique<Model<NodeDefT>>(std::move(def))}
    {}

    NodeDefinition(NodeDefinition const& other)
        : _pimpl{other._pimpl->clone()}
    {}
    auto operator=(NodeDefinition const& other) -> NodeDefinition&
    {
        auto tmp = NodeDefinition{other};
        std::swap(_pimpl, tmp._pimpl);
        return *this;
    }
    NodeDefinition(NodeDefinition&&) noexcept                    = default;
    auto operator=(NodeDefinition&&) noexcept -> NodeDefinition& = default;
    ~NodeDefinition()                                            = default;

private:
    struct Concept { // NOLINT(*-special-member-functions)
        virtual ~Concept() = default;

        [[nodiscard]] virtual auto get_name() const -> std::string           = 0;
        [[nodiscard]] virtual auto clone() const -> std::unique_ptr<Concept> = 0;
    };

    template<NodeDefinition_Concept NodeDefT>
    struct Model : public Concept {
        explicit Model(NodeDefT def)
            : _node_def{std::move(def)}
        {}

        [[nodiscard]] auto get_name() const -> std::string override
        {
            return _node_def.name();
        }

        [[nodiscard]] auto clone() const -> std::unique_ptr<Concept> override
        {
            return std::make_unique<Model>(*this);
        }

        NodeDefT _node_def;
    };

private:
    std::unique_ptr<Concept> _pimpl;
};

} // namespace Cool