#pragma once

#include <Cool/Variables/Variables.h>

namespace Cool {

template<typename T>
struct InputDefinition {
    std::string                name{};
    std::optional<std::string> description{};
    T                          default_value{};
    VariableMetadata<T>        metadata{};
    int                        desired_color_space{0}; // HACK in order to know which color space to convert to when sending the value to a shader. Only used by Color input.
};

#include <Cool/Variables/generated/AnyInputDefinition.inl>

} // namespace Cool