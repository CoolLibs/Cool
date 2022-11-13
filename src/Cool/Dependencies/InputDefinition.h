#pragma once

#include <Cool/Variables/Variables.h>

namespace Cool {

template<typename T>
struct InputDefinition {
    std::string         name{};
    VariableMetadata<T> metadata{};
};

#include <Cool/Variables/generated/AnyInputDefinition.inl>

} // namespace Cool