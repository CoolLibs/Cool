#include "Clock.h"

namespace Cool {

void Clock::play()
{
    if (_is_playing)
        return;
    _is_playing = true;
    on_play();
}

void Clock::pause()
{
    _is_playing = false;
}

void Clock::set_playing(bool wants_to_play)
{
    if (wants_to_play)
        play();
    else
        pause();
}

void Clock::toggle_play_pause()
{
    if (_is_playing)
        pause();
    else
        play();
}

auto Clock::delta_time() const -> Time
{
    return _delta_time;
}

auto Clock::time() const -> Time
{
    return _time;
}

void Clock::set_time(Time new_time, bool force_delta_time_to_ignore_the_change)
{
    if (!force_delta_time_to_ignore_the_change)
        _extra_delta_time += new_time - _time;
    _time = new_time;
}

void Clock::update()
{
    auto const real_time_elapsed = _is_playing
                                       ? update_and_get_delta_time() * _time_speed.value()
                                       : 0s;
    _time += real_time_elapsed;
    _delta_time       = real_time_elapsed + _extra_delta_time;
    _extra_delta_time = 0s;
}

} // namespace Cool