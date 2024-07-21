#pragma once
#include <filesystem>
#include "Cool/StrongTypes/Color.h"

namespace Cool {

template<typename T>
concept NodesCategoryConfig_Concept = requires(T const config_const, T config) { // clang-format off
    { config_const.color() } -> std::convertible_to<Color>;
    { config_const.number_of_main_input_pins() } -> std::convertible_to<size_t>;
    { config.imgui_popup() } -> std::convertible_to<bool>;
}; // clang-format on

class NodesCategoryConfig {
public:
    [[nodiscard]] auto color() const -> Color { return _pimpl->color(); }
    [[nodiscard]] auto number_of_main_input_pins() const -> size_t { return _pimpl->number_of_main_input_pins(); }
    auto               imgui_popup() -> bool { return _pimpl->imgui_popup(); }

    template<NodesCategoryConfig_Concept NodesCategoryConfigT>
    [[nodiscard]] auto downcast() -> NodesCategoryConfigT&
    {
        return static_cast<Model<NodesCategoryConfigT>&>(*_pimpl)._nodes_category_config;
    }
    template<NodesCategoryConfig_Concept NodesCategoryConfigT>
    [[nodiscard]] auto downcast() const -> NodesCategoryConfigT const&
    {
        return static_cast<Model<NodesCategoryConfigT> const&>(*_pimpl)._nodes_category_config;
    }

public: // Type-erasure implementation details
    NodesCategoryConfig() = default;

    template<NodesCategoryConfig_Concept NodesCategoryConfigT>
    NodesCategoryConfig(NodesCategoryConfigT node) // NOLINT(*-explicit-constructor, *-explicit-conversions) A type-erased object should be implicitly created from objects matching its requirements.
        : _pimpl{std::make_unique<Model<NodesCategoryConfigT>>(std::move(node))}
    {}

    NodesCategoryConfig(NodesCategoryConfig const& other)
        : _pimpl{other._pimpl->clone()}
    {}
    auto operator=(NodesCategoryConfig const& other) -> NodesCategoryConfig&
    {
        auto tmp = NodesCategoryConfig{other};
        std::swap(_pimpl, tmp._pimpl);
        return *this;
    }
    NodesCategoryConfig(NodesCategoryConfig&&) noexcept                    = default;
    auto operator=(NodesCategoryConfig&&) noexcept -> NodesCategoryConfig& = default;
    ~NodesCategoryConfig()                                                 = default;

public:              // Must be public in order for ser20 to register the polymorphic types
    struct Concept { // NOLINT(*-special-member-functions)
        virtual ~Concept() = default;

        [[nodiscard]] virtual auto color() const -> Color                      = 0;
        [[nodiscard]] virtual auto number_of_main_input_pins() const -> size_t = 0;
        virtual auto               imgui_popup() -> bool                       = 0;

        [[nodiscard]] virtual auto clone() const -> std::unique_ptr<Concept> = 0;
    };

    template<NodesCategoryConfig_Concept NodesCategoryConfigT>
    struct Model : public Concept {
        Model() = default;
        explicit Model(NodesCategoryConfigT concrete)
            : _nodes_category_config{std::move(concrete)}
        {}

        [[nodiscard]] auto color() const -> Color override
        {
            return _nodes_category_config.color();
        }
        [[nodiscard]] auto number_of_main_input_pins() const -> size_t override
        {
            return _nodes_category_config.number_of_main_input_pins();
        }
        auto imgui_popup() -> bool override
        {
            return _nodes_category_config.imgui_popup();
        }

        [[nodiscard]] auto clone() const -> std::unique_ptr<Concept> override
        {
            return std::make_unique<Model>(*this);
        }

        NodesCategoryConfigT _nodes_category_config;

    private:
        // Serialization
        friend class ser20::access;
        template<class Archive>
        void serialize(Archive& archive)
        {
            archive(
                ser20::make_nvp("NodesCategoryConfigT", _nodes_category_config)
            );
        }
    };

private:
    std::unique_ptr<Concept> _pimpl;

private:
    // Serialization
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("pimpl", _pimpl)
        );
    }
};

} // namespace Cool