#include "requires_shader_code_generation.h"

namespace Cool {

template<typename T>
auto requires_shader_code_generation(VariableDefinition<T> const&) -> bool
{
#include "generated/requires_shader_code_generation.inl"
}

auto requires_shader_code_generation(AnyVariableDefinition const& def) -> bool
{
    return std::visit([](auto&& def) { return requires_shader_code_generation(def); }, def);
}

} // namespace Cool