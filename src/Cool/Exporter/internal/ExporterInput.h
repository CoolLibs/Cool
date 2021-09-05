#pragma once

#include <Cool/Gpu/RenderTarget.h>

namespace Cool {

struct ExporterInput {
    RenderTarget&                      render_target;
    std::function<void(RenderTarget&)> render_fn;

    void render(ImageSize size)
    {
        render_target.set_size(size);
        render_fn(render_target);
    }
};

} // namespace Cool