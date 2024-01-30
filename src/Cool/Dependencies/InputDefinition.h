#pragma once

#include <Cool/Variables/Variables.h>

namespace Cool {

template<typename T>
struct InputDefinition {
    std::string                name{}; // TODO(Variables) Replace with VariableData
    std::optional<std::string> description{};
    T                          default_value{};        // TODO(Variables) Replace with VariableData
    VariableMetadata<T>        metadata{};             // TODO(Variables) Replace with VariableData
    int                        desired_color_space{0}; // HACK in order to know which color space to convert to when sending the value to a shader. Only used by Color input.
};

#include <Cool/Variables/generated/AnyInputDefinition.inl>

} // namespace Cool