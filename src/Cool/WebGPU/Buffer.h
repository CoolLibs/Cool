#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

class Buffer : public WGPUUnique<wgpu::Buffer> {
public:
    explicit Buffer(wgpu::BufferDescriptor const&);
};

} // namespace Cool