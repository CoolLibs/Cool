#pragma once

namespace Cool {

template<typename T>
concept NodeDefinition_Concept = requires(T node_def) {
                                     // clang-format off
    { node_def.name() } -> std::convertible_to<std::string>;
                                     // clang-format on
                                 };

} // namespace Cool