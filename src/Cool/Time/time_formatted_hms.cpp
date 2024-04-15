#include "time_formatted_hms.h"
#include <string>

namespace Cool {

auto time_formatted_hms(float time_in_sec, bool show_milliseconds, float total_duration) -> std::string
{
    const char* sign = time_in_sec < 0.f ? "-" : "";
    time_in_sec      = std::abs(time_in_sec);

    if (total_duration == 0.f)
        total_duration = time_in_sec;

    auto const t = static_cast<uint32_t>(time_in_sec);
    return sign
           + (total_duration < 60.f
                  ? fmt::format("{}s", t)
              : total_duration < 3600.f
                  ? fmt::format("{}m {:02}s", t / 60, t % 60)
                  : fmt::format("{}h {:02}m {:02}s", t / 3600, (t % 3600) / 60, t % 60)
           )
           + (show_milliseconds
                  ? fmt::format(" {:03}ms", static_cast<uint32_t>(time_in_sec * 1000.f) % 1000)
                  : "");
}

} // namespace Cool