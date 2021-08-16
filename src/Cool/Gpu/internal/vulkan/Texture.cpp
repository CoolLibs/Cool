#include "../../Texture.h"
#include <Cool/Vulkan/Context.h>
#include <imgui/backends/imgui_impl_vulkan.h>

namespace Cool {

Texture::Texture(uint32_t width, uint32_t height, vk::Format format)
    : _vku{
          Cool::Vulkan::context().g_Device,
          Cool::Vulkan::context().memory_properties,
          width,
          height,
          1,
          format}
    , _sampler{vku::SamplerMaker{}.createUnique(Vulkan::context().g_Device)}
{
    _imgui_texture_id = ImGui_ImplVulkan_AddTexture(*_sampler, _vku.imageView(), static_cast<VkImageLayout>(_vku.layout()));
}

Texture::Texture(const ImageData& image_data, vk::Format format)
    : Texture{image_data.width,
              image_data.height,
              format}
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

Texture::Id Texture::id()
{
    return {_vku.image()};
}

} // namespace Cool