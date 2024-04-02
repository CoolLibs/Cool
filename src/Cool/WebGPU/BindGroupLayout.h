#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "WebGPU/webgpu.hpp"

namespace Cool {

class BindGroupLayout : public WGPUUnique<wgpu::BindGroupLayout> {
public:
    explicit BindGroupLayout(wgpu::BindGroupLayoutDescriptor const&);
};

} // namespace Cool