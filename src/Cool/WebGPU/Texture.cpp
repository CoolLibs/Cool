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
    return texture_from_pixels(image.size(), image.data_span());
}

auto texture_from_pixels(img::Size size, std::span<uint8_t const> data) -> Texture
{
    wgpu::TextureDescriptor texture_desc;
    texture_desc.dimension       = wgpu::TextureDimension::_2D;
    texture_desc.size            = {size.width(), size.height(), 1};
    texture_desc.mipLevelCount   = 1;
    texture_desc.sampleCount     = 1;
    texture_desc.format          = wgpu::TextureFormat::RGBA8Unorm;
    texture_desc.usage           = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
    texture_desc.viewFormatCount = 0;
    texture_desc.viewFormats     = nullptr;

    auto res = Texture{texture_desc};
    res.set_image(/*channels count=*/data.size() / size.width() / size.height(), data);

    return res;
}

void Texture::set_image(uint32_t color_components_count, std::span<uint8_t const> data)
{
    assert(data.size() % color_components_count == 0);
    assert(data.size() / color_components_count == width() * height());

    wgpu::ImageCopyTexture destination;
    destination.texture  = handle();
    destination.mipLevel = 0;
    destination.origin   = {0, 0, 0};
    destination.aspect   = wgpu::TextureAspect::All; // only relevant for depth/stencil textures

    wgpu::TextureDataLayout source;
    source.offset       = 0;
    source.bytesPerRow  = color_components_count * width();
    source.rowsPerImage = height();

    webgpu_context().queue.writeTexture(destination, data.data(), data.size(), source, _desc.size);
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