#include "time_formatted_hms.h"
#include <string>

namespace Cool {

auto time_formatted_hms(Time time, bool show_milliseconds, Time total_duration) -> std::string
{
    auto nanoseconds = time.as_nanoseconds_int64_t();

    const char* sign = nanoseconds < 0 ? "-" : "";
    nanoseconds      = std::abs(nanoseconds);

    if (total_duration == Time{})
        total_duration = time;
    auto const total_nanoseconds = total_duration.as_nanoseconds_int64_t();

    static constexpr int64_t s       = 1'000'000'000; // Converts seconds to nanoseconds;
    auto const               seconds = nanoseconds / s;
    return sign
           + (total_nanoseconds < 60 * s
                  ? fmt::format("{}s", seconds)
              : total_nanoseconds < 3600 * s
                  ? fmt::format("{}m {:02}s", seconds / 60, seconds % 60)
                  : fmt::format("{}h {:02}m {:02}s", seconds / 3600, (seconds % 3600) / 60, seconds % 60)
           )
           + (show_milliseconds
                  ? fmt::format(" {:03}ms", (nanoseconds / 1'000'000) % 1000)
                  : "");
}

} // namespace Cool