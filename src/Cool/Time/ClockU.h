#pragma once
#include "Clock.h"

namespace Cool::ClockU {

void imgui_timeline(Cool::Clock&, std::function<void()> const& extra_widgets, std::function<void()> const& on_time_reset);

} // namespace Cool::ClockU
