#pragma once
#include "WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class BindGroup : public WGPUUnique<wgpu::BindGroup> {
public:
    BindGroup() = default;
    explicit BindGroup(wgpu::BindGroupDescriptor const&);
};

} // namespace Cool