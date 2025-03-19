
#include "VideoPlayerSettings.hpp"
#include "smart/smart.hpp"

namespace Cool {
auto VideoPlayerSettings::apply(Time time, double duration) const -> Time
{
    // TODO(Video) Implement this to improve performance when playing video backward: https://www.opencv-srf.com/2017/12/play-video-file-backwards.html
    double time_in_seconds = (time - start_time).as_seconds_double() * playback_speed.value;
    switch (loop_mode)
    {
    case VideoPlayerLoopMode::None:
    {
        if (time_in_seconds < 0. || time_in_seconds >= duration)
            return Time{-1s};
        break;
    }
    case VideoPlayerLoopMode::Loop:
    {
        time_in_seconds = smart::mod(time_in_seconds, duration);
        break;
    }
    case VideoPlayerLoopMode::Hold:
    {
        time_in_seconds = std::clamp(time_in_seconds, 0., duration);
        break;
    }
    }
    return Time::seconds(time_in_seconds);
}
} // namespace Cool