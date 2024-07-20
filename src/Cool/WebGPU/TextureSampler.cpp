#include "TextureSampler.hpp"
#include <webgpu/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"
#include "TextureSamplerDescriptor.hpp"

namespace Cool {

TextureSampler::TextureSampler(wgpu::SamplerDescriptor const& desc)
    : WGPUUnique<wgpu::Sampler>{webgpu_context().device.createSampler(desc)}
{
}

static auto wgpu_address_mode(RepeatMode mode) -> wgpu::AddressMode
{
    switch (mode)
    {
    case RepeatMode::None:
        return wgpu::AddressMode::ClampToEdge; // TODO(WebGPU) We want the equivalent of ClampToBorder with a transparent border
    case RepeatMode::Clamp:
        return wgpu::AddressMode::ClampToEdge;
    case RepeatMode::Mirror:
        return wgpu::AddressMode::MirrorRepeat;
    case RepeatMode::Mosaic:
        return wgpu::AddressMode::Repeat;
    }
    assert(false);
    return wgpu::AddressMode::ClampToEdge;
}

static auto wgpu_interpolation_mode(InterpolationMode mode) -> wgpu::FilterMode
{
    switch (mode)
    {
    case InterpolationMode::NearestNeighbour:
        return wgpu::FilterMode::Nearest;
    case InterpolationMode::Linear:
        return wgpu::FilterMode::Linear;
    }
    assert(false);
    return wgpu::FilterMode::Nearest;
}

static auto make_wgpu_descriptor(TextureSamplerDescriptor const& desc) -> wgpu::SamplerDescriptor
{
    wgpu::SamplerDescriptor sampler_desc;
    sampler_desc.addressModeU  = wgpu_address_mode(desc.repeat_mode);
    sampler_desc.addressModeV  = wgpu_address_mode(desc.repeat_mode);
    sampler_desc.addressModeW  = wgpu_address_mode(desc.repeat_mode);
    sampler_desc.magFilter     = wgpu_interpolation_mode(desc.interpolation_mode);
    sampler_desc.minFilter     = wgpu_interpolation_mode(desc.interpolation_mode);
    sampler_desc.mipmapFilter  = wgpu::MipmapFilterMode::Linear;
    sampler_desc.lodMinClamp   = 0.f;
    sampler_desc.lodMaxClamp   = 1.f;
    sampler_desc.compare       = wgpu::CompareFunction::Undefined;
    sampler_desc.maxAnisotropy = 1;
    return sampler_desc;
}

TextureSampler::TextureSampler(TextureSamplerDescriptor const& desc)
    : TextureSampler(make_wgpu_descriptor(desc))
{
}

} // namespace Cool