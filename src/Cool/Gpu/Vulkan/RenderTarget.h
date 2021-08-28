#pragma once
#if defined(__COOL_APP_VULKAN)

#include "../RenderTargetInfo.h"
#include "Texture.h"

namespace Cool::Vulkan {

class RenderTarget {
public:
    RenderTarget(vk::Format format = vk::Format::eR8G8B8A8Srgb, uint32_t width = 1, uint32_t height = 1);

    void render(std::function<void(vk::CommandBuffer& cb)> render_fn);

    RenderTargetInfo info() const;
    vk::RenderPass   render_pass() const { return *_render_pass; }
    uint32_t         width() const { return _texture.image().extent().width; }
    uint32_t         height() const { return _texture.image().extent().height; }
    vk::Format       format() const { return _texture.image().format(); }
    const Texture&   texture() const { return _texture; }

    void        imgui_window();
    void        resize(uint32_t width, uint32_t height);
    ImTextureID imgui_texture_id() const { return _texture.imgui_texture_id(); }

private:
    void build();
    void build_render_pass();
    void build_framebuffer();

private:
    Texture               _texture;
    vk::UniqueRenderPass  _render_pass;
    vk::UniqueFramebuffer _framebuffer;
    vk::CommandBuffer     _command_buffer;
};

} // namespace Cool::Vulkan

#endif