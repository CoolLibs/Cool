#if defined(__COOL_APP_VULKAN)

#include "Window_Vulkan.h"
#include <Cool/Gpu/Vulkan/Context.h>
#include <Cool/Gpu/Vulkan/internal/check_result.h>
#include <imgui/backends/imgui_impl_vulkan.h>

namespace Cool {

Window_Vulkan::Window_Vulkan(GLFWwindow* window)
    : _base{window}
{
}

Window_Vulkan::~Window_Vulkan()
{
    ImGui_ImplVulkanH_DestroyWindow(
        Vulkan::context().g_Instance,
        Vulkan::context().g_Device,
        &_vulkan_window_state.g_MainWindowData,
        Vulkan::context().g_Allocator);
}

void Window_Vulkan::check_for_swapchain_rebuild()
{
    if (_vulkan_window_state.g_SwapChainRebuild) {
        int width, height;
        glfwGetFramebufferSize(_base.glfw(), &width, &height);
        if (width > 0 && height > 0) {
            ImGui_ImplVulkan_SetMinImageCount(_vulkan_window_state.g_MinImageCount);
            _vulkan_window_state.g_MainWindowData.Width  = width;
            _vulkan_window_state.g_MainWindowData.Height = height;
            rebuild_swapchain();
            _vulkan_window_state.g_MainWindowData.FrameIndex = 0;
            _vulkan_window_state.g_SwapChainRebuild          = false;
        }
    }
}

void Window_Vulkan::FrameRender(ImDrawData* draw_data, std::function<void(vk::CommandBuffer)> render_fn)
{
    auto*    wd = &_vulkan_window_state.g_MainWindowData;
    VkResult err;

    VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
    VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    err                                   = vkAcquireNextImageKHR(Vulkan::context().g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
        _vulkan_window_state.g_SwapChainRebuild = true;
        return;
    }
    Vulkan::check_result(err);

    ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
    {
        err = vkWaitForFences(Vulkan::context().g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
        Vulkan::check_result(err);

        err = vkResetFences(Vulkan::context().g_Device, 1, &fd->Fence);
        Vulkan::check_result(err);
    }
    {
        err = vkResetCommandPool(Vulkan::context().g_Device, fd->CommandPool, 0);
        Vulkan::check_result(err);
        VkCommandBufferBeginInfo info = {};
        info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
        Vulkan::check_result(err);
    }
    {
        VkRenderPassBeginInfo info    = {};
        info.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        info.renderPass               = wd->RenderPass;
        info.framebuffer              = fd->Framebuffer;
        info.renderArea.extent.width  = wd->Width;
        info.renderArea.extent.height = wd->Height;
        info.clearValueCount          = 1;
        info.pClearValues             = &wd->ClearValue;
        vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }

    // Record dear imgui primitives into command buffer
    ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);
    render_fn(fd->CommandBuffer);

    // Submit command buffer
    vkCmdEndRenderPass(fd->CommandBuffer);
    {
        VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        VkSubmitInfo         info       = {};
        info.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        info.waitSemaphoreCount         = 1;
        info.pWaitSemaphores            = &image_acquired_semaphore;
        info.pWaitDstStageMask          = &wait_stage;
        info.commandBufferCount         = 1;
        info.pCommandBuffers            = &fd->CommandBuffer;
        info.signalSemaphoreCount       = 1;
        info.pSignalSemaphores          = &render_complete_semaphore;

        err = vkEndCommandBuffer(fd->CommandBuffer);
        Vulkan::check_result(err);
        err = vkQueueSubmit(Vulkan::context().g_Queue, 1, &info, fd->Fence);
        Vulkan::check_result(err);
    }
}

void Window_Vulkan::FramePresent()
{
    auto* wd = &_vulkan_window_state.g_MainWindowData;
    if (_vulkan_window_state.g_SwapChainRebuild)
        return;
    VkSemaphore      render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
    VkPresentInfoKHR info                      = {};
    info.sType                                 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    info.waitSemaphoreCount                    = 1;
    info.pWaitSemaphores                       = &render_complete_semaphore;
    info.swapchainCount                        = 1;
    info.pSwapchains                           = &wd->Swapchain;
    info.pImageIndices                         = &wd->FrameIndex;
    VkResult err                               = vkQueuePresentKHR(Vulkan::context().g_Queue, &info);
    if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
        _vulkan_window_state.g_SwapChainRebuild = true;
        return;
    }
    Vulkan::check_result(err);
    wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
}

void Window_Vulkan::rebuild_swapchain()
{
    ImGui_ImplVulkanH_CreateOrResizeWindow(
        Vulkan::context().g_Instance,
        Vulkan::context().g_PhysicalDevice,
        Vulkan::context().g_Device,
        &_vulkan_window_state.g_MainWindowData,
        Vulkan::context().g_QueueFamily,
        Vulkan::context().g_Allocator,
        _vulkan_window_state.g_MainWindowData.Width,
        _vulkan_window_state.g_MainWindowData.Height,
        _vulkan_window_state.g_MinImageCount);
}

void Window_Vulkan::cap_framerate(bool should_cap)
{
    if (_present_mode_mailbox_is_avaible) {
        _vulkan_window_state.g_MainWindowData.PresentMode = should_cap ? VK_PRESENT_MODE_FIFO_KHR
                                                                       : VK_PRESENT_MODE_MAILBOX_KHR;
        vkDeviceWaitIdle(Vulkan::context().g_Device); // Safety measure but we could probably do more efficient (who cares though ?)
        rebuild_swapchain();
    }
    else {
        Log::warn("[Window_Vulkan::cap_framerate] Cannot un-cap framerate because VK_PRESENT_MODE_MAILBOX_KHR is not available.");
    }
}

bool Window_Vulkan::framerate_is_capped()
{
    return _vulkan_window_state.g_MainWindowData.PresentMode == VK_PRESENT_MODE_FIFO_KHR ||
           _vulkan_window_state.g_MainWindowData.PresentMode == VK_PRESENT_MODE_FIFO_RELAXED_KHR;
}

bool Window_Vulkan::imgui_cap_framerate()
{
    bool should_cap_framerate = framerate_is_capped();
    bool checkbox_triggered   = ImGui::Checkbox("Framerate capped", &should_cap_framerate);
    if (checkbox_triggered) {
        cap_framerate(should_cap_framerate);
    }
    return checkbox_triggered;
}

} // namespace Cool

#endif