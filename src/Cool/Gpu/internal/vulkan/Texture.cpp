#include "../../Texture.h"
#include <Cool/Vulkan/Context.h>
#include <imgui/backends/imgui_impl_vulkan.h>

namespace Cool {

Texture::Texture(const ImageData& image_data)
    : _vku{
          Cool::Vulkan::context().g_Device,
          Cool::Vulkan::context().memory_properties,
          image_data.width,
          image_data.height}
    , _sampler{vku::SamplerMaker{}.createUnique(Vulkan::context().g_Device)}
{
    _vku.upload(
        Vulkan::context().g_Device,
        image_data.data.get(),
        image_data.data_array_size(),
        Vulkan::context().command_pool,
        Vulkan::context().memory_properties,
        Vulkan::context().g_Queue);
    _imgui_texture_id = ImGui_ImplVulkan_AddTexture(*_sampler, _vku.imageView(), static_cast<VkImageLayout>(_vku.layout()));
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