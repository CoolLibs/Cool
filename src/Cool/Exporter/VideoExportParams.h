#pragma once

namespace Cool {

struct VideoExportParams {
    float fps       = 30.f;
    float beginning = 0.f;
    float end       = 10.f;

    void imgui();
};

} // namespace Cool