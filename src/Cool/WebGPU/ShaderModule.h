#pragma once
#include "WebGPU/webgpu.hpp"

namespace Cool {

class ShaderModule {
public:
    explicit ShaderModule(std::string_view wgsl_source_code);

    auto handle() { return _shader_module; } // TODO(WebGPU) Is this indeed just an int that is cheap to copy around?

private:
    wgpu::ShaderModule _shader_module;
};

} // namespace Cool