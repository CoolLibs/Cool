#include "Buffer.h"
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

Buffer::Buffer(wgpu::BufferDescriptor const& desc)
    : WGPUUnique<wgpu::Buffer>{webgpu_context().device.createBuffer(desc)}
{}

} // namespace Cool