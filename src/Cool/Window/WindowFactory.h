#pragma once

#include "internal/WindowFactory_Base.h"

#if defined(__COOL_APP_OPENGL)
#include "internal/WindowFactory_OpenGL.h"
namespace Cool {
using WindowFactory = WindowFactory_Base<WindowFactory_OpenGL>;
} // namespace Cool

#elif defined(__COOL_APP_VULKAN)
#include "internal/WindowFactory_Vulkan.h"
namespace Cool {
using WindowFactory = WindowFactory_Base<WindowFactory_Vulkan>;
} // namespace Cool

#endif