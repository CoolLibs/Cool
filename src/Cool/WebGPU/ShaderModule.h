#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "WebGPU/webgpu.hpp"

namespace Cool {

class ShaderModule : public WGPUUnique<wgpu::ShaderModule> {
public:
    explicit ShaderModule(std::string_view wgsl_source_code);
};

} // namespace Cool