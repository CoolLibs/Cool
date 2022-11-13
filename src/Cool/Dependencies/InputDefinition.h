#pragma once

#include <Cool/Variables/Variables.h>

namespace Cool {

template<typename T>
struct InputDefinition {
    std::string                name{};
    std::optional<std::string> description{};
    T                          default_value{};
    VariableMetadata<T>        metadata{};
};

#include <Cool/Variables/generated/AnyInputDefinition.inl>

} // namespace Cool