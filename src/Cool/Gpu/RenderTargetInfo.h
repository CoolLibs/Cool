#pragma once

#include <Cool/App/internal/RectSizePos.h>

namespace Cool {

struct RenderTargetInfo {
    RectSizePos viewport;
#if defined(__COOL_APP_VULKAN)
    VkRenderPass render_pass;
#endif
};

} // namespace Cool