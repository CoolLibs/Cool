#pragma once

namespace Cool {

inline auto hack_get_global_time_in_seconds() -> float& // TODO(Video) Cleaner way to send current time to video players?
{
    static float f{0.f};
    return f;
}
inline auto hack_get_is_exporting() -> bool& // TODO(Video) Cleaner way to send info to video players?
{
    static bool b{false};
    return b;
}

} // namespace Cool