#include "Clock.h"

namespace Cool {

void Clock::play()
{
    _is_playing = true;
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
    {
        pause();
    }
    else
    {
        play();
    }
}

} // namespace Cool