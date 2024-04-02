#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "WebGPU/webgpu.hpp"

namespace Cool {

class Buffer : public WGPUUnique<wgpu::Buffer> {
public:
    explicit Buffer(wgpu::BufferDescriptor const&);
};

} // namespace Cool