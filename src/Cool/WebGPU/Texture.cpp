#include "Texture.h"
#include <img/src/Load.h>
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"

namespace Cool {

Texture::Texture(wgpu::TextureDescriptor const& desc)
    : WGPUUnique<wgpu::Texture>{webgpu_context().device.createTexture(desc)}
    , _desc{desc}
{}

auto load_texture(std::filesystem::path const& path) -> Texture
{
    auto const image = img::load(path);

    wgpu::TextureDescriptor texture_desc;
    texture_desc.dimension       = wgpu::TextureDimension::_2D;
    texture_desc.size            = {image.width(), image.height(), 1};
    texture_desc.mipLevelCount   = 1;
    texture_desc.sampleCount     = 1;
    texture_desc.format          = wgpu::TextureFormat::RGBA8Unorm;
    texture_desc.usage           = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
    texture_desc.viewFormatCount = 0;
    texture_desc.viewFormats     = nullptr;

    auto res = Texture{texture_desc};
    res.set_image(image.channels_count(), image.data(), image.data_size());

    return res;
}

void Texture::set_image(uint32_t color_components_count, std::vector<uint8_t> const& data)
{
    set_image(color_components_count, data.data(), data.size());
}

void Texture::set_image(uint32_t color_components_count, uint8_t const* data, size_t data_size)
{
    assert(data_size % color_components_count == 0);
    assert(data_size / color_components_count == _desc.size.width * _desc.size.height);

    wgpu::ImageCopyTexture destination;
    destination.texture  = handle();
    destination.mipLevel = 0;
    destination.origin   = {0, 0, 0};
    destination.aspect   = wgpu::TextureAspect::All; // only relevant for depth/stencil textures

    wgpu::TextureDataLayout source;
    source.offset       = 0;
    source.bytesPerRow  = color_components_count * _desc.size.width;
    source.rowsPerImage = _desc.size.height;

    webgpu_context().queue.writeTexture(destination, data, data_size, source, _desc.size);
}

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