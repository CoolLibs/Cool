#pragma once
#include "Clock.h"

namespace Cool {

class Clock_FixedTimestep : public Clock {
public:
    explicit Clock_FixedTimestep(double fps)
        : _delta_time{Time::seconds(1. / fps)}
    {}

private:
    auto update_and_get_delta_time() -> Time override { return _delta_time; }

private:
    Time _delta_time;
};

} // namespace Cool