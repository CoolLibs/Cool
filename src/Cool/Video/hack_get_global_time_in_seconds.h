#pragma once

namespace Cool {

inline auto hack_get_global_time_in_seconds() -> float& // TODO(Video) Cleaner way to send current time to video players?
{
    static float f{};
    return f;
}
inline auto hack_get_is_dragging_time_in_the_timeline() -> bool& // TODO(Video) Cleaner way to send current time to video players?
{
    static bool b{};
    return b;
}

} // namespace Cool