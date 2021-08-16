#include "../RenderTarget.h"
#include <imgui/backends/imgui_impl_vulkan.h>
#include "../Context.h"

namespace Cool::Vulkan {

RenderTarget::RenderTarget(uint32_t width, uint32_t height, vk::Format format)
    : _texture{width, height, format}
{
    build();
}

RenderTargetInfo RenderTarget::info() const
{
    RenderTargetInfo info;
    info.viewport = RectSizePos{
        static_cast<int>(width()),
        static_cast<int>(height())};
    info.render_pass = *_render_pass;
    return info;
}

void RenderTarget::build()
{
    build_render_pass();
    build_framebuffer();
}

void RenderTarget::build_render_pass()
{
    vku::RenderpassMaker _rpm;
    _rpm.attachmentBegin(format());
    _rpm.attachmentInitialLayout(vk::ImageLayout::eUndefined);
    _rpm.attachmentLoadOp(vk::AttachmentLoadOp::eClear);
    _rpm.attachmentFinalLayout(vk::ImageLayout::eGeneral);
    _rpm.subpassBegin(vk::PipelineBindPoint::eGraphics);
    _rpm.subpassColorAttachment(vk::ImageLayout::eColorAttachmentOptimal, 0);
    _render_pass = _rpm.createUnique(Vulkan::context().g_Device);
}

void RenderTarget::build_framebuffer()
{
    vk::ImageView attachments[] = {_texture.image().imageView()};

    vk::FramebufferCreateInfo fbci{
        {},
        *_render_pass,
        1,
        attachments,
        width(),
        height(),
        1,
    };

    _framebuffer = vk::Device{Vulkan::context().g_Device}.createFramebufferUnique(fbci);
}

} // namespace Cool::Vulkan
