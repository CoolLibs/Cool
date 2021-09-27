#pragma once
#if defined(__COOL_APP_VULKAN)

#include <Cool/Image/ImageSize.h>
#include "../RenderTargetInfo.h"
#include "../Vulkan/Texture.h"

namespace Cool {

class RenderTarget_ImplVulkan {
public:
    using RenderFuncType = std::function<void(vk::CommandBuffer& cb)>;
    RenderTarget_ImplVulkan(vk::Format format = vk::Format::eR8G8B8A8Srgb, ImageSize size = {});

    void render(RenderFuncType render_fn);

    RenderTargetInfo       info() const;
    vk::RenderPass         render_pass() const { return *_render_pass; }
    uint32_t               width() const { return _texture.image().extent().width; }
    uint32_t               height() const { return _texture.image().extent().height; }
    ImageSize              size() const { return {width(), height()}; }
    vk::Format             format() const { return _texture.image().format(); }
    const Vulkan::Texture& texture() const { return _texture; }
    ImageData              download_pixels() const;

    void        imgui_window();
    void        resize(ImageSize size);
    ImTextureID imgui_texture_id() const { return _texture.imgui_texture_id(); }

private:
    void build();
    void build_render_pass();
    void build_framebuffer();

private:
    Vulkan::Texture       _texture;
    vk::UniqueRenderPass  _render_pass;
    vk::UniqueFramebuffer _framebuffer;
    vk::CommandBuffer     _command_buffer;
};

} // namespace Cool

#endif