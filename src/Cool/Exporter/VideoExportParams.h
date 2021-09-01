#pragma once

namespace Cool {

struct VideoExportParams {
    float _fps                        = 30.f;
    float _sequence_begin_time_in_sec = 0.f;
    float _sequence_end_time_in_sec   = 10.f;

    void imgui();
};

} // namespace Cool