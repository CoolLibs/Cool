#pragma once
#include <Cool/Gpu/RenderTarget.h>

namespace Cool {

struct Polaroid {
    // The target that we will render to
    RenderTarget& render_target;
    // The function that renders the desired image
    std::function<void(RenderTarget&, float time, float delta_time)> render_fn;

    void render(float time, float delta_time)
    {
        render_fn(render_target, time, delta_time);
    }
    void render(float time, float delta_time, img::Size size)
    {
        render_target.set_size(size);
        render(time, delta_time);
    }
};

} // namespace Cool