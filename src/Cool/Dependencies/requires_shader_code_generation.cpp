#include "requires_shader_code_generation.h"

namespace Cool {

template<typename T>
auto requires_shader_code_generation(InputDefinition<T> const&) -> bool
{
#include <Cool/Variables/generated/requires_shader_code_generation.inl>
}

auto requires_shader_code_generation(AnyInputDefinition const& def) -> bool
{
    return std::visit([](auto&& def) { return requires_shader_code_generation(def); }, def);
}

} // namespace Cool