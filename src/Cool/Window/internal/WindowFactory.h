#pragma once

#include "WindowFactory_Base.h"

#if defined(COOL_OPENGL)
#include "WindowFactory_OpenGL.h"
namespace Cool {
using WindowFactory = WindowFactory_Base<WindowFactory_OpenGL>;
} // namespace Cool

#elif defined(COOL_VULKAN)
#include "WindowFactory_Vulkan.h"
namespace Cool {
using WindowFactory = WindowFactory_Base<WindowFactory_Vulkan>;
} // namespace Cool

#endif