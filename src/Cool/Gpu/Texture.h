#pragma once

#include <Cool/Image/ImageData.h>

namespace Cool {

class Texture {
public:
    struct Id;
    Texture() = default;
    Texture(const ImageData& image_data);
    Texture(std::string_view path);

    Id          id();
    ImTextureID imgui_texture_id() const { return _imgui_texture_id; }

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