#include "../ShaderSource.h"

namespace Cool {

ShaderSource::ShaderSource(std::string_view vulkan_glsl_source, ShaderKind shader_kind)
    : _vulkan_glsl_source{vulkan_glsl_source}, _shader_kind{shader_kind}
{
}

std::string ShaderSource::to_opengl_glsl() const
{
    return _vulkan_glsl_source;
}

} // namespace Cool