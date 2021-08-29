#pragma once
#if defined(__COOL_APP_VULKAN)

#include <Cool/Image/ImageData.h>

namespace Cool::Vulkan {

class Texture {
public:
    struct Id;
    Texture() = default;
    Texture(ImageSize              size,
            vk::Format             format                           = vk::Format::eR8G8B8A8Unorm,
            vk::ImageLayout        layout_when_read_by_imgui_shader = vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::ImageUsageFlagBits additional_usage_flags           = (vk::ImageUsageFlagBits)0);
    Texture(const ImageData&       image_data,
            vk::Format             format                           = vk::Format::eR8G8B8A8Unorm,
            vk::ImageLayout        layout_when_read_by_imgui_shader = vk::ImageLayout::eShaderReadOnlyOptimal,
            vk::ImageUsageFlagBits additional_usage_flags           = (vk::ImageUsageFlagBits)0);
    Texture(std::string_view path);

    /**
     * @brief Recreates the underlying image with a new size. Warning : the data is lost and the image is reset to black !
     * 
     * @param size
     */
    void resize(ImageSize size);

    Id                         id();
    ImTextureID                imgui_texture_id() const { return _imgui_texture_id; }
    const vku::TextureImage2D& image() const { return _vku; }
    vku::TextureImage2D&       image() { return _vku; }

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
    vk::ImageLayout     _layout_when_read_by_imgui_shader;
    ImTextureID         _imgui_texture_id;
};

} // namespace Cool::Vulkan

#endif