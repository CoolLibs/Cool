#if defined(__COOL_APP_VULKAN)

#include "../../Texture.h"
#include <imgui/backends/imgui_impl_vulkan.h>
#include "../Context.h"

namespace Cool::Vulkan {

Texture::Texture(uint32_t width, uint32_t height, vk::Format format, vk::ImageLayout layout_when_read_by_imgui_shader, vk::ImageUsageFlagBits additional_usage_flags)
    : _vku{
          Cool::Vulkan::context().g_Device,
          Cool::Vulkan::context().memory_properties,
          width,
          height,
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
    : Texture{image_data.width,
              image_data.height,
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

void Texture::resize(uint32_t width, uint32_t height)
{
    _vku = vku::TextureImage2D{
        Cool::Vulkan::context().g_Device,
        Cool::Vulkan::context().memory_properties,
        width,
        height,
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

} // namespace Cool::Vulkan

#endif