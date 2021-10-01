#pragma once

#include "Clock.h"

namespace Cool {

class Clock_FixedTimestep : public Clock {
public:
    Clock_FixedTimestep(float fps, float initial_time = 0.f);
    ~Clock_FixedTimestep() = default;

    float delta_time() const override;
    float time() const override;

    void set_time(float new_time) override;
    void update() override;

private:
    const float  _dt;
    unsigned int _frames_count; // store time as an int to avoid floating point imprecisions when adding a small number (dt) to a big one (time) [don't know if this is really necessary though]
};

} // namespace Cool