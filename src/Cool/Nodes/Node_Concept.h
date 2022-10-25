#pragma once
#include <ranges>
#include "Pin.h"

namespace Cool {

template<typename T>
concept Node_Concept = true;
// requires(T node)
// {
//     std::range<decltype(node.input_pins())>;
//     std::range<decltype(node.output_pins())>;
// };

} // namespace Cool