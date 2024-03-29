#pragma once

#if defined(COOL_OPENGL)

#include "Window_ImplOpenGL.h"
namespace Cool {
using Window_Impl = Window_ImplOpenGL;
}

#elif defined(COOL_VULKAN)

#include "internal/Window_ImplVulkan.h"
namespace Cool {
using Window_Impl = Window_Vulkan;
}

#endif