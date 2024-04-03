#include "Texture.h"
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

Texture::Texture(wgpu::TextureDescriptor const& desc)
    : WGPUUnique<wgpu::Texture>{webgpu_context().device.createTexture(desc)}
    , _desc{desc}
{}

auto Texture::entire_texture_view() const -> TextureView const&
{
    if (!_entire_texture_view.has_value())
    {
        wgpu::TextureViewDescriptor textureViewDesc;
        textureViewDesc.aspect          = wgpu::TextureAspect::All;
        textureViewDesc.baseArrayLayer  = 0;
        textureViewDesc.arrayLayerCount = 1;
        textureViewDesc.baseMipLevel    = 0;
        textureViewDesc.mipLevelCount   = 1;
        textureViewDesc.dimension       = wgpu::TextureViewDimension::_2D;
        textureViewDesc.format          = _desc.format;
        _entire_texture_view.emplace(handle().createView(textureViewDesc));
    }
    return *_entire_texture_view;
}

} // namespace Cool