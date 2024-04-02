#include "BindGroup.h"
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

BindGroup::BindGroup(wgpu::BindGroupDescriptor const& desc)
    : WGPUUnique<wgpu::BindGroup>{webgpu_context().device.createBindGroup(desc)}
{}

} // namespace Cool