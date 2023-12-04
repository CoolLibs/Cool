#include "Clock_Realtime.h"
#include <chrono>

namespace Cool {

static auto std_time() -> std::chrono::steady_clock::time_point
{
    return std::chrono::steady_clock::now();
}

Clock_Realtime::Clock_Realtime()
    : _offset_with_std_time(std_time())
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
    auto const now = is_playing() ? std_time() : _std_time_when_paused;
    return std::chrono::duration<float>{now - _offset_with_std_time}.count();
}

void Clock_Realtime::set_time(float new_time)
{
    _offset_with_std_time += std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(time() - new_time));
}

void Clock_Realtime::update()
{
    _prev_time  = _time;
    _time       = compute_time();
    _delta_time = _time - _prev_time; // Must be before `_current_std_time = std_time();`, otherwise the delta will not be computed properly when the time is frozen because the user is using the input text of the timeline to set the time value.
}

void Clock_Realtime::play()
{
    if (is_playing())
        return;
    Clock::play();
    _offset_with_std_time += std_time() - _std_time_when_paused;
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