#pragma once

#if defined(__COOL_APP_OPENGL)

#include "internal/Window_OpenGL.h"
namespace Cool {
using Window = Window_OpenGL;
}

#elif defined(__COOL_APP_VULKAN)

#include "internal/Window_Vulkan.h"
namespace Cool {
using Window = Window_Vulkan;
}

#endif