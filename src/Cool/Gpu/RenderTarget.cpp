#include "RenderTarget.h"

namespace Cool {

void RenderTarget::render(std::function<void()> const& render_fn)
{
    resize_if_necessary();
    // _impl.render(render_fn);
}

void RenderTarget::resize_if_necessary()
{
    if (needs_resizing())
    {
        // _impl.resize(desired_size());
    }
}

} // namespace Cool