#pragma once
#include "Cool/WebGPU/WGPUUnique.h"
#include "TextureSamplerDescriptor.hpp"
#include "webgpu/webgpu.hpp"

namespace Cool {

class TextureSampler : public WGPUUnique<wgpu::Sampler> {
public:
    explicit TextureSampler(wgpu::SamplerDescriptor const& desc);
    explicit TextureSampler(TextureSamplerDescriptor const& desc);
};

} // namespace Cool