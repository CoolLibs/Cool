#include "JsonAutoSerializer.hpp"
#include "Cool/DebugOptions/DebugOptions.h"

namespace Cool::internal {

auto wants_to_log_internal_warnings() -> bool
{
    return DebugOptions::log_internal_warnings();
}

} // namespace Cool::internal