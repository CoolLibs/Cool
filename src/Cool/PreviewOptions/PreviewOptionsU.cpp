#include "PreviewOptionsU.h"

namespace Cool::RenderStateU {

void constrain_render_target_size(const PreviewOptions& preview_options, RenderTarget& render_target)
{
    render_target.set_constrained_size(preview_options.constrained_size(render_target.imgui_window_size()));
    render_target.set_is_aspect_ratio_constrained(preview_options.wants_to_constrain_aspect_ratio());
}

} // namespace Cool::RenderStateU