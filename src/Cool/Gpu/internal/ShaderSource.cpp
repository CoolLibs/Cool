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

static shaderc_shader_kind shader_kind_cool2shaderc(Cool::ShaderKind shader_kind)
{
    switch (shader_kind) {
    case Cool::ShaderKind::Vertex:
        return shaderc_shader_kind::shaderc_vertex_shader;
    case Cool::ShaderKind::Fragment:
        return shaderc_shader_kind::shaderc_fragment_shader;
    case Cool::ShaderKind::Geometry:
        return shaderc_shader_kind::shaderc_geometry_shader;
    case Cool::ShaderKind::Compute:
        return shaderc_shader_kind::shaderc_compute_shader;
    }
}

shaderc::SpvCompilationResult ShaderSource::to_vulkan_spirv() const
{
    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;

    auto res = compiler.CompileGlslToSpv(_vulkan_glsl_source, shader_kind_cool2shaderc(_shader_kind), "Unknown");

    if (!res.GetErrorMessage().empty()) {
        Log::warn(res.GetErrorMessage());
    }

    return res;
}

} // namespace Cool