#pragma once

#include <Cool/Gpu/RenderTarget.h>

namespace Cool {

struct Polaroid {
    // The target that we will render to
    RenderTarget& render_target;
    // The function that renders the desired image
    std::function<void(RenderTarget&, float time)> render_fn;

    void render(ImageSize size, float time)
    {
        render_target.set_size(size);
        render_fn(render_target, time);
    }
};

} // namespace Cool