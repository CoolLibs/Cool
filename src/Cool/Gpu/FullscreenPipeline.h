#pragma once

#if defined(COOL_OPENGL)
#include "OpenGL/FullscreenPipeline.h"
namespace Cool {
using FullscreenPipeline = OpenGL::FullscreenPipeline;
}

#elif defined(COOL_VULKAN)
#include "Vulkan/FullscreenPipeline.h"
namespace Cool {
using FullscreenPipeline = Vulkan::FullscreenPipeline;
}

#endif