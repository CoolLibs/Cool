#pragma once

#include <Cool/Gpu/RenderTargetInfo.h>
#include <Cool/Gpu/Texture.h>
#include <vku/vku.hpp>

namespace Cool::Vulkan {

class RenderTarget {
public:
    RenderTarget(uint32_t width, uint32_t height, vk::Format format = vk::Format::eR8G8B8A8Srgb);
    RenderTargetInfo info() const;

    uint32_t   width() const { return _texture.image().extent().width; }
    uint32_t   height() const { return _texture.image().extent().height; }
    vk::Format format() const { return _texture.image().format(); }

    const Texture& texture() const { return _texture; }

private:
    void build();
    void build_render_pass();
    void build_framebuffer();

public:
    Texture               _texture;
    vk::UniqueRenderPass  _render_pass;
    vk::UniqueFramebuffer _framebuffer;
};

} // namespace Cool::Vulkan
