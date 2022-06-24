#include "Clock_FixedTimestep.h"

namespace Cool {

Clock_FixedTimestep::Clock_FixedTimestep(float fps, float initial_time)
    : _dt(1.f / fps)
{
    _frames_count = static_cast<unsigned int>(std::round(initial_time * fps));
}

float Clock_FixedTimestep::delta_time() const
{
    if (is_playing())
    {
        return _dt;
    }
    else
    {
        return 0.f;
    }
}

float Clock_FixedTimestep::time() const
{
    return static_cast<float>(_frames_count) * _dt;
}

void Clock_FixedTimestep::set_time(float new_time)
{
    _frames_count = static_cast<unsigned int>(std::round(new_time / _dt));
}

void Clock_FixedTimestep::update()
{
    if (is_playing())
    {
        _frames_count++;
    }
}

} // namespace Cool