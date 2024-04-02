#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "WebGPU/webgpu.hpp"

namespace Cool {

class BindGroup : public WGPUUnique<wgpu::BindGroup> {
public:
    BindGroup() = default;
    explicit BindGroup(wgpu::BindGroupDescriptor const&);
};

} // namespace Cool