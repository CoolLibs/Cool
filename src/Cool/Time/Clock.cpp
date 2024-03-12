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
    return static_cast<float>(time_in_nanoseconds / 1'000'000'000) // NOLINT(bugprone-integer-division)
           + static_cast<float>(time_in_nanoseconds % 1'000'000'000) / static_cast<float>(1'000'000'000);
}
static auto as_nanoseconds_from_float_in_seconds(float time_in_seconds) -> int64_t
{
    float const floor = std::floor(time_in_seconds);
    float const fract = time_in_seconds - floor;

    return static_cast<int64_t>(floor) * 1'000'000'000
           + static_cast<int64_t>(fract * 1'000'000'000.f);
}

auto Clock::delta_time_in_seconds() const -> float
{
    return as_float_in_seconds_from_nanoseconds(_delta_time_in_nanoseconds);
}

auto Clock::time_in_seconds() const -> float
{
    return as_float_in_seconds_from_nanoseconds(_time_in_nanoseconds);
}

void Clock::set_time(float new_time_in_seconds, bool force_delta_time_to_ignore_the_change)
{
    auto const new_time_in_nanoseconds = as_nanoseconds_from_float_in_seconds(new_time_in_seconds);

    if (!force_delta_time_to_ignore_the_change)
        _extra_delta_time_in_nanoseconds += new_time_in_nanoseconds - _time_in_nanoseconds;
    _time_in_nanoseconds = new_time_in_nanoseconds;
}

void Clock::update()
{
    int64_t const real_time_elapsed = [&]() -> int64_t { // IIFE
        if (!_is_playing)
            return 0;

        int64_t const delta_time = update_and_get_delta_time_in_nanoseconds();
        if (_time_speed.value().value == 1.f) // This is the most common case, and we don't want to waste precision by converting back and forth between float and int64_t.
            return delta_time;
        return static_cast<int64_t>(static_cast<float>(delta_time) * _time_speed.value().value);
    }();
    _time_in_nanoseconds += real_time_elapsed;
    _delta_time_in_nanoseconds = real_time_elapsed
                                 + _extra_delta_time_in_nanoseconds;
    _extra_delta_time_in_nanoseconds = 0;
}

} // namespace Cool