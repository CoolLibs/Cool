#if defined(__COOL_APP_VULKAN)

#include "../ShaderModule.h"
#include <Cool/File/File.h>
#include <shaderc/shaderc.hpp>
#include "../Context.h "

namespace Cool::Vulkan {

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

static vku::ShaderModule shader_module_from_glsl_file(std::string_view file_path, Cool::ShaderKind shader_kind)
{
    std::string             glsl_source = File::to_string(file_path);
    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;

    auto res = compiler.CompileGlslToSpv(glsl_source, shader_kind_cool2shaderc(shader_kind), file_path.data());

    if (!res.GetErrorMessage().empty()) {
        Log::warn(res.GetErrorMessage());
    }

    return vku::ShaderModule{Vulkan::context().g_Device, res.begin(), res.end()};
}

ShaderModule::ShaderModule(std::string_view file_path, Cool::ShaderKind shader_kind)
    : _vku_shader_module{shader_module_from_glsl_file(file_path, shader_kind)}
{
}

} // namespace Cool::Vulkan

#endif