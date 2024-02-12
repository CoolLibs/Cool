#pragma once
#include "SharedVariableDefinition.h"

namespace Cool {

auto always_requires_shader_code_generation(AnySharedVariableDefinition const&) -> bool;
template<typename T>
auto always_requires_shader_code_generation(Variable<T> const&) -> bool
{
#include <Cool/Variables/generated/always_requires_shader_code_generation.inl>
}

} // namespace Cool