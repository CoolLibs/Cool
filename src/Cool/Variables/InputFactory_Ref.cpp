#include "InputFactory_Ref.h"

namespace Cool {

auto InputFactory_Ref::make(AnyVariableDefinition const& def, DirtyFlag const& dirty_flag) const -> AnySharedVariableId
{
    return std::visit([&](auto&& def) { return AnySharedVariableId{make(def, dirty_flag)}; }, def);
}

} // namespace Cool