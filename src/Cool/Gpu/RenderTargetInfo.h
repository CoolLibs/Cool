#pragma once

#include "Viewport.h"

namespace Cool {

struct RenderTargetInfo {
    Viewport viewport;
#if defined(__COOL_APP_VULKAN)
    VkRenderPass render_pass;
#endif
};

} // namespace Cool