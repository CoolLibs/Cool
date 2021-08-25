#pragma once

#if defined(__COOL_APP_OPENGL)

#include "OpenGL/Texture.h"
namespace Cool {
using Texture = OpenGL::Texture;
}

#elif defined(__COOL_APP_VULKAN)

#include "Vulkan/Texture.h"
namespace Cool {
using Texture = Vulkan::Texture;
}

#endif