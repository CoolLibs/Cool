#if defined(__COOL_APP_VULKAN)

#include "../ShaderModule.h"
#include <Cool/File/File.h>
#include "../Context.h "
#include "../ShaderSource.h"

namespace Cool::Vulkan {

static vku::ShaderModule shader_module_from_glsl_file(std::string_view file_path, Cool::ShaderKind shader_kind)
{
    auto res = ShaderSource{File::to_string(file_path), shader_kind}.to_spirv();

    return vku::ShaderModule{Vulkan::context().g_Device, res.begin(), res.end()};
}

ShaderModule::ShaderModule(std::string_view file_path, Cool::ShaderKind shader_kind)
    : _vku_shader_module{shader_module_from_glsl_file(file_path, shader_kind)}
{
}

} // namespace Cool::Vulkan

#endif