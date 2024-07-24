#include "BindGroup.hpp"
#include <webgpu/webgpu.hpp>
#include "Cool/WebGPU/WebGPUContext.hpp"

namespace Cool {

BindGroup::BindGroup(wgpu::BindGroupDescriptor const& desc)
    : WGPUUnique<wgpu::BindGroup>{webgpu_context().device.createBindGroup(desc)}
{}

} // namespace Cool