#include "Clock_Realtime.h"

namespace Cool {

Clock_Realtime::Clock_Realtime()
    : _initial_std_time(std_time()), _current_std_time(std_time())
{
}

auto Clock_Realtime::delta_time() const -> float
{
    return _delta_time;
}

auto Clock_Realtime::time() const -> float
{
    return _time;
}

auto Clock_Realtime::compute_time() const -> float
{
    auto const delta = std::chrono::duration<float>{_current_std_time - _initial_std_time};
    return delta.count() - _offset_with_std_time;
}

void Clock_Realtime::set_time(float new_time)
{
    _offset_with_std_time += time() - new_time;
}

void Clock_Realtime::update()
{
    _prev_time  = _time;
    _time       = compute_time();
    _delta_time = _time - _prev_time; // Must be before `_current_std_time = std_time();`, otherwise the delta will not be computed properly when the time is frozen because the user is using the input text of the timeline to set the time value.

    if (is_playing())
        _current_std_time = std_time();
}

void Clock_Realtime::play()
{
    if (is_playing())
        return;
    Clock::play();
    auto const delta = std::chrono::duration<float>{std_time() - _std_time_when_paused};
    _offset_with_std_time += delta.count();
    update();
}

void Clock_Realtime::pause()
{
    if (!is_playing())
        return;
    Clock::pause();
    _std_time_when_paused = std_time();
}

} // namespace Cool