#pragma once
#if defined(COOL_VULKAN)

#include "../ShaderDescription.h"

namespace Cool::Vulkan {

class ShaderModule {
public:
    explicit ShaderModule(const ShaderDescription& shader_description);

    const vku::ShaderModule& vku() const { return _vku_shader_module; }

private:
    vku::ShaderModule _vku_shader_module;
};

} // namespace Cool::Vulkan

#endif