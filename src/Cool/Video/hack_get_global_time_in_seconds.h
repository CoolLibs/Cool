#pragma once

namespace Cool {

inline auto hack_get_global_time_in_seconds() -> float& // TODO(Video) Cleaner way to send current time to video players?
{
    static float f{};
    return f;
}

} // namespace Cool