#include "BindGroupLayout.h"
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

static auto make_bind_group_layout(std::span<BindGroupLayoutEntry const> entries)
{
    std::vector<wgpu::BindGroupLayoutEntry> bindings(entries.size(), wgpu::Default);

    for (size_t i = 0; i < entries.size(); ++i)
    {
        bindings[i].binding    = i;
        bindings[i].visibility = wgpu::ShaderStage::Compute;

        switch (entries[i])
        {
        case BindGroupLayoutEntry::Read_Buffer:
        {
            bindings[i].buffer.type = wgpu::BufferBindingType::ReadOnlyStorage;
            break;
        }
        case BindGroupLayoutEntry::ReadWrite_Buffer:
        case BindGroupLayoutEntry::Write_Buffer: // There is no such thing as a write-only buffer
        {
            bindings[i].buffer.type = wgpu::BufferBindingType::Storage;
            break;
        }
        case BindGroupLayoutEntry::Read_Texture:
        {
            bindings[i].texture.sampleType    = wgpu::TextureSampleType::Float;
            bindings[i].texture.viewDimension = wgpu::TextureViewDimension::_2D;
            break;
        }
        case BindGroupLayoutEntry::Write_Texture:
        {
            bindings[i].storageTexture.access        = wgpu::StorageTextureAccess::WriteOnly;
            bindings[i].storageTexture.format        = wgpu::TextureFormat::RGBA8Unorm;
            bindings[i].storageTexture.viewDimension = wgpu::TextureViewDimension::_2D;
            break;
        }
        }
    }

    wgpu::BindGroupLayoutDescriptor bind_group_layout_desc;
    bind_group_layout_desc.entryCount = static_cast<uint32_t>(bindings.size());
    bind_group_layout_desc.entries    = bindings.data();

    return webgpu_context().device.createBindGroupLayout(bind_group_layout_desc);
}

BindGroupLayout::BindGroupLayout(wgpu::BindGroupLayoutDescriptor const& desc)
    : WGPUUnique<wgpu::BindGroupLayout>{webgpu_context().device.createBindGroupLayout(desc)}
{}

BindGroupLayout::BindGroupLayout(std::span<BindGroupLayoutEntry const> entries)
    : WGPUUnique<wgpu::BindGroupLayout>{make_bind_group_layout(entries)}
{}

} // namespace Cool