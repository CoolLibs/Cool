#pragma once

#include "internal/RenderTarget_Base.h"

#if defined(COOL_OPENGL)
#include "internal/RenderTarget_ImplOpenGL.h"
namespace Cool {
using RenderTarget = RenderTarget_Base<RenderTarget_ImplOpenGL>;
}

#elif defined(COOL_VULKAN)
#include "internal/RenderTarget_ImplVulkan.h"
namespace Cool {
using RenderTarget = RenderTarget_Base<RenderTarget_ImplVulkan>;
}

#endif