#include "Clock_Realtime.h"
#include <chrono>

namespace Cool {

auto Clock_Realtime::update_and_get_delta_time_in_nanoseconds() -> int64_t
{
    auto const now        = std::chrono::steady_clock::now();
    auto const delta_time = now - _last_update;
    _last_update          = now;
    return std::chrono::nanoseconds{delta_time}.count();
}

void Clock_Realtime::on_play()
{
    _last_update = std::chrono::steady_clock::now();
}

} // namespace Cool