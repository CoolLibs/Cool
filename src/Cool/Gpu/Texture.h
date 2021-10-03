#pragma once

#if defined(COOL_OPENGL)
#include "OpenGL/Texture.h"
namespace Cool {
using Texture = OpenGL::Texture;
}

#elif defined(COOL_VULKAN)
#include "Vulkan/Texture.h"
namespace Cool {
using Texture = Vulkan::Texture;
}

#endif