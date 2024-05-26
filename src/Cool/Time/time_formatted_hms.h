#pragma once
#include "Time.h"

namespace Cool {

/// time is displayed as [hours]:[minutes]:[seconds]. Hours and minutes might not be displayed if they are 0 for both time_in_sec and total_duration.
/// `total_duration`: Optional: the total length of the time interval that `time` is part of. (for example `time` might be a timestamp in a video, and total_duration would be the duration of the video). It allows the formatting to know whether it should write hours and minutes or not, in order for the display to be consistent across the whole duration of total_duration. Leave as 0 if you don't want or need this behaviour.
auto time_formatted_hms(Time time, bool show_milliseconds = false, Time total_duration = {}) -> std::string;

} // namespace Cool