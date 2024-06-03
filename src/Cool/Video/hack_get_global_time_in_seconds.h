#pragma once
#include "Cool/Time/Time.h"

namespace Cool {

inline auto hack_get_global_time_in_seconds() -> Time& // TODO(Video) Cleaner way to send current time to video players?
{
    static auto t = Time{0s};
    return t;
}
inline auto hack_get_is_exporting() -> bool& // TODO(Video) Cleaner way to send info to video players?
{
    static bool b{false};
    return b;
}

} // namespace Cool