#pragma once

#include <Cool/Gpu/RenderTarget.h>
#include "RenderState.h"

namespace Cool::RenderStateU {

void constrain_render_target_size(const RenderState& render_state, RenderTarget& render_target);

} // namespace Cool::RenderStateU