#pragma once
#include "Clock.h"

namespace Cool::ClockU {

void imgui_timeline(Cool::Clock&, std::function<void()> const &on_time_reset);
void imgui_display_time(Cool::Clock& clock);

} // namespace Cool::ClockU
