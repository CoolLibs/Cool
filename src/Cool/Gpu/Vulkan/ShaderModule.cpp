#if defined(COOL_VULKAN)

#include "ShaderModule.h"
#include <Cool/File/File.h>
#include "Context.h "
#include "ShaderSource.h"

namespace Cool::Vulkan {

static vku::ShaderModule shader_module_from_glsl_source(const ShaderDescription& shader_description)
{
    auto res = ShaderSource::to_spirv(shader_description);
    return vku::ShaderModule{Vulkan::context().g_Device, res.begin(), res.end()};
}

ShaderModule::ShaderModule(const ShaderDescription& shader_description)
    : _vku_shader_module{shader_module_from_glsl_source(shader_description)}
{
}

} // namespace Cool::Vulkan

#endif