#pragma once
#include <chrono>
#include "Clock.h"

namespace Cool {

class Clock_Realtime : public Clock {
private:
    auto update_and_get_delta_time_in_nanoseconds() -> int64_t override;
    void on_play() override;

private:
    std::chrono::steady_clock::time_point _last_update{std::chrono::steady_clock::now()};
};

} // namespace Cool