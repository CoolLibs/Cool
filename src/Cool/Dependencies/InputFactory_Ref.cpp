#include "InputFactory_Ref.h"

namespace Cool {

auto InputFactory_Ref::make(AnyInputDefinition const& def, DirtyFlag dirty_flag, DirtyFlag secondary_dirty_flag) const -> AnyInput
{
    return std::visit([&](auto&& def) { return AnyInput{make(def, dirty_flag, secondary_dirty_flag)}; }, def);
}

} // namespace Cool