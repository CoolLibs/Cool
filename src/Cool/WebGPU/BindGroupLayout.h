#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

enum class BindGroupLayoutEntry {
    Read_Buffer,
    ReadWrite_Buffer,
    Write_Buffer,
    Read_Texture,
    Write_Texture,
};

class BindGroupLayout : public WGPUUnique<wgpu::BindGroupLayout> {
public:
    explicit BindGroupLayout(wgpu::BindGroupLayoutDescriptor const&);
    explicit BindGroupLayout(std::span<BindGroupLayoutEntry const> entries);
};

} // namespace Cool