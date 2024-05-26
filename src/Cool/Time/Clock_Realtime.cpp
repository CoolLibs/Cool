#include "Clock_Realtime.h"
#include <chrono>

namespace Cool {

auto Clock_Realtime::update_and_get_delta_time() -> Time
{
    auto const now        = std::chrono::steady_clock::now();
    auto const delta_time = now - _last_update;
    _last_update          = now;
    return delta_time;
}

void Clock_Realtime::on_play()
{
    _last_update = std::chrono::steady_clock::now();
}

} // namespace Cool