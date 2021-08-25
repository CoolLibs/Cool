#pragma once
#if defined(__COOL_APP_VULKAN)

#include <Cool/Gpu/ShaderKind.h>

namespace Cool::Vulkan {

class ShaderModule {
public:
    ShaderModule(std::string_view file_path, Cool::Gpu::ShaderKind shader_kind);

    const vku::ShaderModule& vku() const { return _vku_shader_module; }

private:
    vku::ShaderModule _vku_shader_module;
};

} // namespace Cool::Vulkan

#endif