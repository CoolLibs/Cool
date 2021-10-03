#pragma once
#if defined(COOL_VULKAN)

#include <shaderc/shaderc.hpp>
#include "../ShaderKind.h"

namespace Cool::Vulkan {

class ShaderSource {
public:
    ShaderSource(std::string_view vulkan_glsl_source, ShaderKind shader_kind);

    shaderc::SpvCompilationResult to_spirv() const;
    ShaderKind                    kind() const { return _shader_kind; }

private:
    std::string _vulkan_glsl_source;
    ShaderKind  _shader_kind;
};

} // namespace Cool::Vulkan

#endif