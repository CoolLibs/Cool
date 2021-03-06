#if defined(COOL_VULKAN)

#include "RenderTarget_ImplVulkan.h"
#include <imgui/backends/imgui_impl_vulkan.h>
#include "../Vulkan/Context.h"

namespace Cool {

RenderTarget_ImplVulkan::RenderTarget_ImplVulkan(vk::Format format, img::Size size)
    : _texture{size, format, vk::ImageLayout::eGeneral, vk::ImageUsageFlagBits::eColorAttachment}
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool        = Vulkan::context().command_pool;
    allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer _cb_;
    if (vkAllocateCommandBuffers(Vulkan::context().g_Device, &allocInfo, &_cb_) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
    _command_buffer = _cb_;

    build();
}

void RenderTarget_ImplVulkan::render(RenderFuncType render_fn)
{
    VkRenderPassBeginInfo rp_begin_info    = {};
    rp_begin_info.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin_info.renderPass               = render_pass();
    rp_begin_info.framebuffer              = *_framebuffer;
    rp_begin_info.renderArea.extent.width  = width();
    rp_begin_info.renderArea.extent.height = height();
    rp_begin_info.clearValueCount          = 1;
    ImVec4       clear_color               = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    VkClearValue ClearValue;
    ClearValue.color.float32[0] = 1.f;
    ClearValue.color.float32[1] = 1.f;
    ClearValue.color.float32[2] = 0.f;
    ClearValue.color.float32[3] = 1.f;
    rp_begin_info.pClearValues  = &ClearValue;

    vk::CommandBufferBeginInfo bi{};
    _command_buffer.begin(bi);
    _command_buffer.beginRenderPass(rp_begin_info, vk::SubpassContents::eInline);
    render_fn(_command_buffer);
    _command_buffer.endRenderPass();

    {
        VkPipelineStageFlags wait_stage = {};
        VkSubmitInfo         info       = {};
        info.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount         = 0;
        // info.pWaitSemaphores            = &image_acquired_semaphore;
        info.pWaitDstStageMask  = &wait_stage;
        info.commandBufferCount = 1;
        VkCommandBuffer mcb{_command_buffer};
        info.pCommandBuffers      = &mcb;
        info.signalSemaphoreCount = 0;
        // info.pSignalSemaphores          = &render_complete_semaphore;

        // err = vkEndCommandBuffer(fd->CommandBuffer);
        // Vulkan::check_result(err);
        // err = vkQueueSubmit(Vulkan::context().g_Queue, 1, &info, fd->Fence);
        // Vulkan::check_result(err);

        _command_buffer.end();
        vkQueueSubmit(Vulkan::context().g_Queue, 1, &info, VkFence{});
    }
}

RenderTargetInfo RenderTarget_ImplVulkan::info() const
{
    return RenderTargetInfo{
        .viewport = {
            .size               = size(),
            .bottom_left_corner = {0, 0}},
        .render_pass = *_render_pass};
}

void RenderTarget_ImplVulkan::resize(img::Size size)
{
    _texture.resize(size);
    build_framebuffer();
}

void RenderTarget_ImplVulkan::build()
{
    build_render_pass();
    build_framebuffer();
}

void RenderTarget_ImplVulkan::build_render_pass()
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

void RenderTarget_ImplVulkan::build_framebuffer()
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

img::Image RenderTarget_ImplVulkan::download_pixels() const
{
    return _texture.download_pixels();
}

} // namespace Cool

#endif