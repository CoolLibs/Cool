#include "BindGroupBuilder.hpp"
#include "BindGroupLayout.hpp"
#include "Cool/TextureSource/TextureSamplerLibrary.hpp"
#include "TextureSampler.hpp"
#include "TextureSamplerDescriptor.hpp"


namespace Cool {

#define BUILDER(x)                        \
    _entries.emplace_back(wgpu::Default); \
    _entries.back().binding = binding;    \
    x return *this;

auto BindGroupBuilder::read_texture_2D(int binding, Texture const& texture) -> BindGroupBuilder&
{
    BUILDER(
        _entries.back().textureView = texture.entire_texture_view();
    )
}

auto BindGroupBuilder::sampler(int binding, TextureSampler const& sampler) -> BindGroupBuilder&
{
    BUILDER(
        _entries.back().sampler = sampler;
    )
}

auto BindGroupBuilder::sampler(int binding, TextureSamplerDescriptor const& sampler_desc) -> BindGroupBuilder&
{
    return sampler(binding, texture_sampler_library().get(sampler_desc));
}

// auto BindGroupBuilder::write_texture_2D(int binding) -> BindGroupBuilder&
// {
//     BUILDER(
//         _entries.back().storageTexture.access        = wgpu::StorageTextureAccess::WriteOnly;
//         _entries.back().storageTexture.format        = wgpu::TextureFormat::RGBA8Unorm;
//         _entries.back().storageTexture.viewDimension = wgpu::TextureViewDimension::_2D;
//     )
// }

// auto BindGroupBuilder::read_buffer(int binding) -> BindGroupBuilder&
// {
//     BUILDER(
//         _entries.back().buffer.type = wgpu::BufferBindingType::ReadOnlyStorage;
//     )
// }

// auto BindGroupBuilder::write_buffer(int binding) -> BindGroupBuilder&
// {
//     return read_write_buffer(binding, visibility); // There is no such thing as a write-only buffer in WebGPU
// }

// auto BindGroupBuilder::read_write_buffer(int binding) -> BindGroupBuilder&
// {
//     BUILDER(
//         _entries.back().buffer.type = wgpu::BufferBindingType::Storage;
//     )
// }

auto BindGroupBuilder::build(BindGroupLayout const& layout) const -> BindGroup
{
    auto desc       = wgpu::BindGroupDescriptor{};
    desc.layout     = layout;
    desc.entryCount = _entries.size();
    desc.entries    = _entries.data();
    return Cool::BindGroup{desc};
}

} // namespace Cool