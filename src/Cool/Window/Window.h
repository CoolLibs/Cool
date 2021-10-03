#pragma once

#if defined(COOL_OPENGL)

#include "internal/Window_OpenGL.h"
namespace Cool {
using Window = Window_OpenGL;
}

#elif defined(COOL_VULKAN)

#include "internal/Window_Vulkan.h"
namespace Cool {
using Window = Window_Vulkan;
}

#endif