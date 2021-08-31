#include "RenderStateU.h"

namespace Cool::RenderStateU {

void constrain_render_target_size(const RenderState& render_state, RenderTarget& render_target)
{
    render_target.set_constrained_size(render_state.constrained_size(render_target.imgui_window_size()));
}

} // namespace Cool::RenderStateU