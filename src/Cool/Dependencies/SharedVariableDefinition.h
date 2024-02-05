#pragma once
#include <Cool/Variables/Variables.h>
#include "Cool/Variables/Variable.h"

namespace Cool {

template<typename T>
struct SharedVariableDefinition {
    VariableData<T>            var_data{};
    std::optional<std::string> description{};
    int                        desired_color_space{0}; // HACK in order to know which color space to convert to when sending the value to a shader. Only used by Color input.
};

#include <Cool/Variables/generated/AnySharedVariableDefinition.inl>

} // namespace Cool