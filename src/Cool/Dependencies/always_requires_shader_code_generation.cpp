#include "always_requires_shader_code_generation.h"

namespace Cool {

template<typename T>
auto always_requires_shader_code_generation(SharedVariableDefinition<T> const&) -> bool
{
#include <Cool/Variables/generated/always_requires_shader_code_generation.inl>
}

auto always_requires_shader_code_generation(AnySharedVariableDefinition const& def) -> bool
{
    return std::visit([](auto&& def) { return always_requires_shader_code_generation(def); }, def);
}

} // namespace Cool