#pragma once

#if defined(COOL_OPENGL)
#include "WindowFactory_ImplOpenGL.h"
namespace Cool {
using WindowFactory_Impl = WindowFactory_ImplOpenGL;
} // namespace Cool

#elif defined(COOL_VULKAN)
#include "WindowFactory_ImplVulkan.h"
namespace Cool {
using WindowFactory_Impl = WindowFactory_ImplVulkan;
} // namespace Cool
#endif