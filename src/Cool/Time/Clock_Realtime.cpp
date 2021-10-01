#include "Clock_Realtime.h"

namespace Cool {

Clock_Realtime::Clock_Realtime()
    : _initial_time(std_time()), _last_time(std_time()), _current_time(std_time())
{
}

float Clock_Realtime::delta_time() const
{
    if (is_playing()) {
        std::chrono::duration<float> dt = _current_time - _last_time;
        return dt.count();
    }
    else {
        return 0.f;
    }
}

float Clock_Realtime::time() const
{
    std::chrono::duration<float> delta = _current_time - _initial_time;
    return delta.count() - _offset_with_std_time;
}

void Clock_Realtime::set_time(float new_time)
{
    _offset_with_std_time += time() - new_time;
}

void Clock_Realtime::update()
{
    if (is_playing()) {
        _last_time    = _current_time;
        _current_time = std_time();
    }
}

void Clock_Realtime::play()
{
    Clock::play();
    std::chrono::duration<float> delta = std_time() - _time_when_paused;
    _offset_with_std_time += delta.count();
    update();
}

void Clock_Realtime::pause()
{
    Clock::pause();
    _time_when_paused = std_time();
}

} // namespace Cool