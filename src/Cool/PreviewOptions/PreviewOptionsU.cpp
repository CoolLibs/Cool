#include "PreviewOptionsU.h"

namespace Cool::RenderStateU {

void constrain_render_target_size(const PreviewOptions& preview_options, RenderTarget& render_target)
{
    render_target.set_constrained_size(preview_options.constrained_size(render_target.imgui_window_size()));
}

} // namespace Cool::RenderStateU