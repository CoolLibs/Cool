#pragma once

#include <Cool/Image/ImageData.h>

namespace Cool {

class Texture {
public:
    struct Id;
    Texture() = default;
    Texture(uint32_t               width,
            uint32_t               height,
            vk::Format             format                 = vk::Format::eR8G8B8A8Unorm,
            vk::ImageUsageFlagBits additional_usage_flags = (vk::ImageUsageFlagBits)0);
    Texture(const ImageData&       image_data,
            vk::Format             format                 = vk::Format::eR8G8B8A8Unorm,
            vk::ImageUsageFlagBits additional_usage_flags = (vk::ImageUsageFlagBits)0);
    Texture(std::string_view path);

    Id                         id();
    ImTextureID                imgui_texture_id() const { return _imgui_texture_id; }
    const vku::TextureImage2D& image() const { return _vku; }

    struct Id {
        operator ImTextureID() const
        {
            return reinterpret_cast<ImTextureID>(id);
        }
#if defined(__COOL_APP_OPENGL)
        GLuint id;
#elif defined(__COOL_APP_VULKAN)
        VkImage id;
#endif
    };

private:
    vku::TextureImage2D _vku;
    vk::UniqueSampler   _sampler;
    ImTextureID         _imgui_texture_id;
};

} // namespace Cool