#pragma once
#include "Clock.h"

namespace Cool {

class Clock_FixedTimestep : public Clock {
public:
    explicit Clock_FixedTimestep(float fps)
        : _delta_time_in_nanoseconds{static_cast<int64_t>(1'000'000'000.f / fps)} // TODO(TimeSpeed) Wrong speed
    {}

private:
    auto update_and_get_delta_time_in_nanoseconds() -> int64_t override { return _delta_time_in_nanoseconds; }

private:
    int64_t _delta_time_in_nanoseconds;
};

} // namespace Cool