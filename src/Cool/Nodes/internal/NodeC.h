#pragma once
#include <ranges>
#include "Pin.h"

namespace Cool {

template<typename T>
concept NodeC = true;
// requires(T node)
// {
//     std::range<decltype(node.input_pins())>;
//     std::range<decltype(node.output_pins())>;
// };

class Node { // TODO(JF) Remove and use a template parameter
public:
    auto input_pins() const -> const auto& { return _input_pins; }
    auto input_pins() -> auto& { return _input_pins; }
    auto output_pins() const -> const auto& { return _output_pins; }
    auto output_pins() -> auto& { return _output_pins; }

private:
    std::vector<Pin> _input_pins;
    std::vector<Pin> _output_pins;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // archive(
        //     cereal::make_nvp("Base Module", cereal::base_class<Module>(this)),
        //     cereal::make_nvp("Node Editor", _nodes_editor)
        // );
    }
};

struct NodesConfig {
    // using Node = Node;
    static auto name(Node const&) -> const char* { return "Some Node"; }
};

} // namespace Cool