#pragma once

#include <Cool/App/internal/RectSizePos.h>

namespace Cool {

struct RenderTargetInfo {
    RectSizePos viewport;
    VkRenderPass render_pass;
};

}