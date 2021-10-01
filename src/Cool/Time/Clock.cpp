#include "Clock.h"

namespace Cool {

void Clock::play()
{
    assert(!_is_playing);
    _is_playing = true;
}

void Clock::pause()
{
    assert(_is_playing);
    _is_playing = false;
}

void Clock::toggle_play_pause()
{
    if (_is_playing)
        pause();
    else
        play();
}

} // namespace Cool