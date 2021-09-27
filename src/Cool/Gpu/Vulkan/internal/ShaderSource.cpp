#if defined(__COOL_APP_VULKAN)

#include "../ShaderSource.h"

namespace Cool::Vulkan {

ShaderSource::ShaderSource(std::string_view vulkan_glsl_source, ShaderKind shader_kind)
    : _vulkan_glsl_source{vulkan_glsl_source}, _shader_kind{shader_kind}
{
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
    default:
        Log::error("[ShaderSource::shader_kind_cool2shaderc] Unkown enum value \"{}\"", static_cast<int>(shader_kind));
        return shaderc_shader_kind::shaderc_vertex_shader;
    }
}

shaderc::SpvCompilationResult ShaderSource::to_spirv() const
{
    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;
    options.AddMacroDefinition("COOL_VULKAN");

    auto res = compiler.CompileGlslToSpv(_vulkan_glsl_source, shader_kind_cool2shaderc(_shader_kind), "Unknown", options);

    if (!res.GetErrorMessage().empty()) {
        Log::warn(res.GetErrorMessage());
    }

    return res;
}

} // namespace Cool::Vulkan

#endif