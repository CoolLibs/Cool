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

static auto as_float_in_seconds_from_nanoseconds(int64_t time_in_nanoseconds) -> float
{
    int64_t const bob = 1'000'000'000; // 10^9
    return static_cast<float>(time_in_nanoseconds / bob)
           + static_cast<float>(time_in_nanoseconds % bob) / static_cast<float>(bob);
}

auto Clock::delta_time_in_seconds() const -> float
{
    return as_float_in_seconds_from_nanoseconds(_delta_time_in_nanoseconds + _extra_delta_time_in_nanoseconds);
}

auto Clock::time_in_seconds() const -> float
{
    return as_float_in_seconds_from_nanoseconds(_time_in_nanoseconds);
}

void Clock::set_time(float new_time_in_seconds, bool force_delta_time_to_ignore_the_change)
{
    auto const floor = std::floor(new_time_in_seconds);
    auto const fract = new_time_in_seconds - floor;
    auto const bob   = static_cast<int64_t>(floor) * 1'000'000'000 // TODO(TimeSpeed) Don't hardcode the 10^9
                     + static_cast<int64_t>(fract * 1'000'000'000);
    if (!force_delta_time_to_ignore_the_change)
    {
        _extra_delta_time_in_nanoseconds      = bob - _time_in_nanoseconds; // TODO(TimeSpeed) store a vector of these, in case we call multiple set time between frames. OR just store time during each update, and compute delta time from that
        _used_extra_delta_time_in_nanoseconds = false;
    }
    _time_in_nanoseconds = bob;
}

void Clock::update()
{
    if (!_is_playing)
    {
        _delta_time_in_nanoseconds = 0;
        return;
    }
    _delta_time_in_nanoseconds = static_cast<int64_t>(
        static_cast<float>(update_and_get_delta_time_in_nanoseconds()) * _time_speed.value().value
    );
    if (_used_extra_delta_time_in_nanoseconds)
        _extra_delta_time_in_nanoseconds = 0;
    _used_extra_delta_time_in_nanoseconds = true;
    _time_in_nanoseconds += _delta_time_in_nanoseconds;
}

} // namespace Cool