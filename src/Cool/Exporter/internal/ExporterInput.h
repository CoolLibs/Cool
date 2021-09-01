#pragma once

#include <Cool/Gpu/RenderTarget.h>

namespace Cool {

struct ExporterInput {
    RenderTarget&                      render_target;
    std::function<void(RenderTarget&)> render_fn;
};

} // namespace Cool