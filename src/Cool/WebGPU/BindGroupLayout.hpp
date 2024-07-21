#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class BindGroupLayout : public WGPUUnique<wgpu::BindGroupLayout> {
public:
    explicit BindGroupLayout(wgpu::BindGroupLayoutDescriptor const&);
};

} // namespace Cool