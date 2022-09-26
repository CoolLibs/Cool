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
};

struct NodesConfig {
    // using Node = Node;
    static auto name(Node const&) -> const char* { return "Some Node"; }
};

} // namespace Cool