#pragma once
#include <Cool/Gpu/RenderTarget.h>
#include "Cool/Time/Time.hpp"

namespace Cool {

struct Polaroid {
    // The target that we will render to
    RenderTarget& render_target;
    // The function that renders the desired image
    std::function<void(RenderTarget&, Time time, Time delta_time)> render_fn;

    void render(Time time, Time delta_time)
    {
        render_fn(render_target, time, delta_time);
    }
    void render(Time time, Time delta_time, img::Size size)
    {
        render_target.set_size(size);
        render(time, delta_time);
    }
};

} // namespace Cool