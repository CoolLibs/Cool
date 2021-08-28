#pragma once

#include "internal/RenderTarget.h"

#if defined(__COOL_APP_OPENGL)
#include "OpenGL/RenderTarget.h"
namespace Cool {
using RenderTarget = RenderTarget_Base<OpenGL::RenderTarget>;
}

#elif defined(__COOL_APP_VULKAN)
#include "Vulkan/RenderTarget.h"
namespace Cool {
using RenderTarget = RenderTarget_Base<Vulkan::RenderTarget>;
}

#endif