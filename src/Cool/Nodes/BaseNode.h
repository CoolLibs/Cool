#pragma once
#include "Pin.h"

namespace Cool {

class BaseNode {
public:
    BaseNode()                                       = default;
    BaseNode(BaseNode const&)                        = delete; // Non-copyable
    BaseNode(BaseNode&&) noexcept                    = delete; // and non-movable
    auto operator=(BaseNode const&) -> BaseNode&     = delete; // because this is
    auto operator=(BaseNode&&) noexcept -> BaseNode& = delete; // a base class.
    virtual ~BaseNode()                              = default;

    [[nodiscard]] auto input_pins() -> auto& { return _input_pins; }
    [[nodiscard]] auto input_pins() const -> auto const& { return _input_pins; }
    [[nodiscard]] auto output_pins() -> auto& { return _output_pins; }
    [[nodiscard]] auto output_pins() const -> auto const& { return _output_pins; }

    [[nodiscard]] virtual auto definition_name() const -> std::string = 0; // TODO(JF) Move the notion of Definition to the base class?
    [[nodiscard]] virtual auto category_name() const -> std::string   = 0; // TODO(JF) Move the notion of Category to the base class?

private:
    std::vector<Cool::InputPin>  _input_pins{};
    std::vector<Cool::OutputPin> _output_pins{};

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Input pins", _input_pins),
            cereal::make_nvp("Output pins", _output_pins)
        );
    }
};

} // namespace Cool