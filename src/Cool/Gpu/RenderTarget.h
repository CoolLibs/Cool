#pragma once

#include "internal/RenderTarget_Base.h"

#if defined(__COOL_APP_OPENGL)
#include "OpenGL/RenderTarget_ImplOpenGL.h"
namespace Cool {
using RenderTarget = RenderTarget_Base<OpenGL::RenderTarget>;
}

#elif defined(__COOL_APP_VULKAN)
#include "Vulkan/RenderTarget_ImplVulkan.h"
namespace Cool {
using RenderTarget = RenderTarget_Base<Vulkan::RenderTarget>;
}

#endif