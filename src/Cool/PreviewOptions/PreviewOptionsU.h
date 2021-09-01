#pragma once

#include <Cool/Gpu/RenderTarget.h>
#include "PreviewOptions.h"

namespace Cool::RenderStateU {

void constrain_render_target_size(const PreviewOptions& preview_options, RenderTarget& render_target);

} // namespace Cool::RenderStateU