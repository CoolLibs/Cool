#pragma once

namespace Cool {

/// time is displayed as [hours]:[minutes]:[seconds]. Hours and minutes might not be displayed if they are 0 for both time_in_sec and total_duration.
/// `time_in_sec`: The time to be displayed, expressed in seconds
/// `total_duration`: Optional: the total length of the time interval that time_in_sec is part of, expressed in seconds. (for example time_in_sec might be a timestamp in a video, and total_duration would be the duration of the video). It allows the formatting to know whether it should write hours and minutes or not, in order for the display to be consistent across the whole duration of total_duration. Leave as 0.f if you don't want or need this behaviour.
auto time_formatted_hms(float time_in_sec, bool show_milliseconds = false, float total_duration = 0.f) -> std::string;

} // namespace Cool