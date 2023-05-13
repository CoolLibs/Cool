#pragma once
#include "imgui-node-editor/ID.h"

namespace Cool {

inline auto as_ed_id(reg::AnyId const& id)
{
    return MyIDType{id};
}

} // namespace Cool