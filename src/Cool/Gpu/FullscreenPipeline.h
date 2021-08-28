#pragma once

#if defined(__COOL_APP_OPENGL)
#include "OpenGL/FullscreenPipeline.h"
namespace Cool {
using FullscreenPipeline = OpenGL::FullscreenPipeline;
}

#elif defined(__COOL_APP_VULKAN)
#include "Vulkan/FullscreenPipeline.h"
namespace Cool {
using FullscreenPipeline = Vulkan::FullscreenPipeline;
}

#endif