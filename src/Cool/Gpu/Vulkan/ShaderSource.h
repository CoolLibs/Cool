#pragma once
#if defined(COOL_VULKAN)

#include <shaderc/shaderc.hpp>
#include "../ShaderDescription.h"

namespace Cool::Vulkan::ShaderSource {

shaderc::SpvCompilationResult to_spirv(const ShaderDescription& shader_description);

} // namespace Cool::Vulkan::ShaderSource

#endif