#include "TimeSpeed.h"

namespace Cool {

auto to_string(TimeSpeed time_speed) -> std::string
{
    return fmt::format("x{:.2f}", time_speed.value);
}

} // namespace Cool