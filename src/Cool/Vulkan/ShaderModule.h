#pragma once

namespace Cool::Vulkan {

class ShaderModule {
public:
    ShaderModule(std::string_view file_path);

    const vku::ShaderModule& vku() const { return _vku_shader_module; }

private:
    vku::ShaderModule _vku_shader_module;
};

} // namespace Cool::Vulkan