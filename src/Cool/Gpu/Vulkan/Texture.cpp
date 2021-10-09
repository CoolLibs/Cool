#if defined(COOL_VULKAN)

#include "../Texture.h"
#include <imgui/backends/imgui_impl_vulkan.h>
#include "Context.h"

namespace Cool::Vulkan {

Texture::Texture(ImageSize size, vk::Format format, vk::ImageLayout layout_when_read_by_imgui_shader, vk::ImageUsageFlagBits additional_usage_flags)
    : _vku{
          Cool::Vulkan::context().g_Device,
          Cool::Vulkan::context().memory_properties,
          size.width(),
          size.height(),
          1,
          format,
          false,
          additional_usage_flags}
    , _sampler{vku::SamplerMaker{}.createUnique(Vulkan::context().g_Device)}
    , _layout_when_read_by_imgui_shader{layout_when_read_by_imgui_shader}
{
    _imgui_texture_id = ImGui_ImplVulkan_AddTexture(*_sampler, _vku.imageView(), static_cast<VkImageLayout>(layout_when_read_by_imgui_shader));
}

Texture::Texture(const ImageData& image_data, vk::Format format, vk::ImageLayout layout_when_read_by_imgui_shader, vk::ImageUsageFlagBits additional_usage_flags)
    : Texture{image_data.size,
              format,
              layout_when_read_by_imgui_shader,
              additional_usage_flags}
{
    _vku.upload(
        Vulkan::context().g_Device,
        image_data.data.get(),
        image_data.data_array_size(),
        Vulkan::context().command_pool,
        Vulkan::context().memory_properties,
        Vulkan::context().g_Queue);
}

Texture::Texture(std::string_view path)
    : Texture{ImageData{path}}
{
}

void Texture::resize(ImageSize size)
{
    vkDeviceWaitIdle(Vulkan::context().g_Device); // We must wait for all curent users of the image to finish before destroying it
    _vku = vku::TextureImage2D{
        Cool::Vulkan::context().g_Device,
        Cool::Vulkan::context().memory_properties,
        size.width(),
        size.height(),
        1,
        _vku.format(),
        false,
        _vku.imageUsageFlags()};
    ImGui_ImplVulkan_UpdateTexture(_imgui_texture_id, *_sampler, _vku.imageView(), static_cast<VkImageLayout>(_layout_when_read_by_imgui_shader));
}

Texture::Id Texture::id()
{
    return {_vku.image()};
}

ImageData Texture::download_pixels() const
{
    auto w  = image().extent().width;
    auto h  = image().extent().height;
    auto sz = nb_channels() * w * h;

    vku::GenericBuffer stagingBuffer{Vulkan::context().g_Device, Vulkan::context().memory_properties, (vk::BufferUsageFlags)vk::BufferUsageFlagBits::eTransferDst, (vk::DeviceSize)sz, vk::MemoryPropertyFlagBits::eHostVisible};
    vku::executeImmediately(Vulkan::context().g_Device, Vulkan::context().command_pool, Vulkan::context().g_Queue, [&](vk::CommandBuffer cb) {
        _vku.setLayout(cb, vk::ImageLayout::eGeneral);
        vk::Buffer          buf = stagingBuffer.buffer();
        vk::BufferImageCopy region{};
        region.bufferOffset     = 0;
        region.imageSubresource = {vk::ImageAspectFlagBits::eColor, 0, 0, 1};
        region.imageExtent      = _vku.extent();
        cb.copyImageToBuffer(_vku.image(), _vku.layout(), buf, {region});
    });
    std::unique_ptr<uint8_t[]> data{new uint8_t[sz]};
    void*                      mem = stagingBuffer.map(Vulkan::context().g_Device);
    memcpy(data.get(), mem, sz);
    stagingBuffer.unmap(Vulkan::context().g_Device);
    return ImageData{
        ImageSize{w, h},
        nb_channels(),
        std::move(data)};
}

} // namespace Cool::Vulkan

#endif