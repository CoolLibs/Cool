#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

struct ShaderModule_CreationArgs {
    std::string_view label;
    std::string_view wgsl_code;
};

class ShaderModule : public WGPUUnique<wgpu::ShaderModule> {
public:
    explicit ShaderModule(ShaderModule_CreationArgs const&);
};

} // namespace Cool