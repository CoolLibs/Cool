#include "BindGroupLayout.h"
#include <webgpu/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

BindGroupLayout::BindGroupLayout(wgpu::BindGroupLayoutDescriptor const& desc)
    : WGPUUnique<wgpu::BindGroupLayout>{webgpu_context().device.createBindGroupLayout(desc)}
{}

#define BUILDER(x)                                                                        \
    _layout_entries.emplace_back(wgpu::Default);                                          \
    _layout_entries.back().binding = binding;                                             \
    assert(visibility.has_value() || _default_visibility.has_value());                    \
    _layout_entries.back().visibility = visibility.value_or(_default_visibility.value()); \
    x return *this;

auto BindGroupLayoutBuilder::read_texture_2D(int binding, std::optional<wgpu::ShaderStageFlags> visibility) -> BindGroupLayoutBuilder&
{
    BUILDER(
        _layout_entries.back().texture.sampleType    = wgpu::TextureSampleType::Float;
        _layout_entries.back().texture.viewDimension = wgpu::TextureViewDimension::_2D;
    )
}

auto BindGroupLayoutBuilder::sampler(int binding, std::optional<wgpu::ShaderStageFlags> visibility) -> BindGroupLayoutBuilder&
{
    BUILDER(
        _layout_entries.back().sampler.type = wgpu::SamplerBindingType::Filtering;
    )
}

auto BindGroupLayoutBuilder::write_texture_2D(int binding, std::optional<wgpu::ShaderStageFlags> visibility) -> BindGroupLayoutBuilder&
{
    BUILDER(
        _layout_entries.back().storageTexture.access        = wgpu::StorageTextureAccess::WriteOnly;
        _layout_entries.back().storageTexture.format        = wgpu::TextureFormat::RGBA8Unorm;
        _layout_entries.back().storageTexture.viewDimension = wgpu::TextureViewDimension::_2D;
    )
}

auto BindGroupLayoutBuilder::read_buffer(int binding, std::optional<wgpu::ShaderStageFlags> visibility) -> BindGroupLayoutBuilder&
{
    BUILDER(
        _layout_entries.back().buffer.type = wgpu::BufferBindingType::ReadOnlyStorage;
    )
}

auto BindGroupLayoutBuilder::write_buffer(int binding, std::optional<wgpu::ShaderStageFlags> visibility) -> BindGroupLayoutBuilder&
{
    return read_write_buffer(binding, visibility); // There is no such thing as a write-only buffer in WebGPU
}

auto BindGroupLayoutBuilder::read_write_buffer(int binding, std::optional<wgpu::ShaderStageFlags> visibility) -> BindGroupLayoutBuilder&
{
    BUILDER(
        _layout_entries.back().buffer.type = wgpu::BufferBindingType::Storage;
    )
}

auto BindGroupLayoutBuilder::build() const -> BindGroupLayout
{
    auto bind_group_layout_desc       = wgpu::BindGroupLayoutDescriptor{};
    bind_group_layout_desc.entryCount = _layout_entries.size();
    bind_group_layout_desc.entries    = _layout_entries.data();
    return Cool::BindGroupLayout{bind_group_layout_desc};
}

} // namespace Cool