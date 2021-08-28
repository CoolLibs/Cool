#pragma once

#include "internal/RenderTarget.h"

#if defined(__COOL_APP_OPENGL)
#include "OpenGL/RenderTarget.h"
namespace Cool {
// using RenderTarget = OpenGL::RenderTarget;
}

#elif defined(__COOL_APP_VULKAN)
#include "Vulkan/RenderTarget.h"
namespace Cool {
using RenderTarget = Vulkan::RenderTarget;
}

#endif