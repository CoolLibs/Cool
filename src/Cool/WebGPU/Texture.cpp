#include "Texture.h"
#include <img/src/Load.h>
#include <img/src/Save.h>
#include <WebGPU/webgpu.hpp>
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/WebGPU/Buffer.h"
#include "Cool/WebGPU/ComputePipeline.h"

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

static auto make_compute_pipeline_to_copy_texture_to_buffer() -> ComputePipeline
{
    return ComputePipeline{{
        .label                    = "[Texture] Copy to buffer",
        .bind_group_layout        = std::vector{BindGroupLayoutEntry::Read_Texture, BindGroupLayoutEntry::Write_Buffer},
        .workgroup_size           = glm::uvec3{8, 8, 1}, // "I suggest we use a workgroup size of 8x8: this treats both X and Y axes symmetrically and sums up to 64 threads, which is a reasonable multiple of a typical warp size." from https://eliemichel.github.io/LearnWebGPU/basic-compute/image-processing/mipmap-generation.html#dispatch
        .wgsl_compute_shader_code = R"wgsl(
@group(0) @binding(0) var in_texture: texture_2d<f32>;
@group(0) @binding(1) var<storage, read_write> out_buffer: array<u32>;

@compute
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    if(any(id.xy >= textureDimensions(in_texture)))
    {
        return;
    }
    let color = textureLoad(in_texture, id.xy, 0);
    out_buffer[id.x + id.y * textureDimensions(in_texture).x] =
            u32(color.r * 255.)
        |  (u32(color.g * 255.) <<8)
        |  (u32(color.b * 255.) <<16)
        |  (u32(color.a * 255.) <<24)
        ;
}
)wgsl",
    }};
}

static auto compute_pipeline_to_copy_texture_to_buffer() -> ComputePipeline&
{
    static auto instance = make_compute_pipeline_to_copy_texture_to_buffer();
    return instance;
}

void Texture::save(std::filesystem::path const& path) const
{
    with_pixels([&](std::span<uint8_t const> data) {
        img::save_png(path, width(), height(), data.data(), 4);
    });
}

void Texture::with_pixels(std::function<void(std::span<uint8_t const>)> const& callback) const
{
    wgpu::BufferDescriptor pixelBufferDesc = wgpu::Default;
    pixelBufferDesc.mappedAtCreation       = false;
    pixelBufferDesc.usage                  = wgpu::BufferUsage::CopySrc | wgpu::BufferUsage::Storage;
    pixelBufferDesc.size                   = width() * height() * 4;
    Buffer pixelBuffer{pixelBufferDesc};

    pixelBufferDesc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead;
    Buffer pixelBuffer2{pixelBufferDesc};

    wgpu::CommandEncoder encoder = webgpu_context().device.createCommandEncoder(wgpu::Default);
    compute_pipeline_to_copy_texture_to_buffer().compute({
        .invocation_count_x = width(),
        .invocation_count_y = height(),
        .bind_group         = {
            /* @binding(0) = */ entire_texture_view(),
            /* @binding(1) = */ pixelBuffer
        },
        .encoder = encoder,
    });

    encoder.copyBufferToBuffer(pixelBuffer, 0, pixelBuffer2, 0, pixelBuffer.handle().getSize());

    webgpu_context().queue.submit(encoder.finish(wgpu::Default));

    bool done{false};
    auto callbackHandle = pixelBuffer2.handle().mapAsync(wgpu::MapMode::Read, 0, pixelBufferDesc.size, [&](wgpu::BufferMapAsyncStatus status) {
        if (status != wgpu::BufferMapAsyncStatus::Success)
        {
            done = true;
            // TODO(WebGPU) Error
            return;
        }
        const unsigned char* pixelData = (const unsigned char*)pixelBuffer2.handle().getConstMappedRange(0, pixelBufferDesc.size);
        callback(std::span{pixelData, width() * height() * 4});

        pixelBuffer2.handle().unmap();
        done = true;
    });
    while (!done) // TODO(WebGPU) Don't block here and provide an async API?
    {
#ifdef WEBGPU_BACKEND_WGPU
        webgpu_context().queue.submit(0, nullptr);
#else
        webgpu_context().device.tick();
#endif
    }
}

} // namespace Cool