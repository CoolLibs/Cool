#include "../ShaderModule.h"
#include <Cool/File/File.h>
#include <shaderc/shaderc.hpp>
#include "../Context.h "

namespace Cool::Vulkan {

static vku::ShaderModule shader_module_from_glsl_file(std::string_view file_path)
{
    std::string glsl_source;
    File::to_string(file_path, &glsl_source);
    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;

    auto res = compiler.CompileGlslToSpv(glsl_source, shaderc_shader_kind::shaderc_fragment_shader, file_path.data());

    Log::warn(res.GetErrorMessage());

    return vku::ShaderModule{Vulkan::context().g_Device, res.begin(), res.end()};
}

ShaderModule::ShaderModule(std::string_view file_path)
    : _vku_shader_module{shader_module_from_glsl_file(file_path)}
{
}

} // namespace Cool::Vulkan