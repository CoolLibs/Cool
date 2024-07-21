#include "BindGroupLayout.hpp"
#include <webgpu/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

BindGroupLayout::BindGroupLayout(wgpu::BindGroupLayoutDescriptor const& desc)
    : WGPUUnique<wgpu::BindGroupLayout>{webgpu_context().device.createBindGroupLayout(desc)}
{}

} // namespace Cool