#include "InputFactory_Ref.h"

namespace Cool {

auto InputFactory_Ref::make(AnyInputDefinition const& def, DirtyFlag const& dirty_flag) const -> AnyInput
{
    return std::visit([&](auto&& def) { return AnyInput{make(def, dirty_flag)}; }, def);
}

} // namespace Cool