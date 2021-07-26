#include "../Window.h"
#include <GLFW/glfw3.h>

namespace Cool {

Window::Window(GLFWwindow* glfw_window, const vku::Framework& vku_framework)
    : _glfw_window{glfw_window}
    , _vku_window{
          vku_framework.instance(),
          vku_framework.device(),
          vku_framework.physicalDevice(),
          vku_framework.graphicsQueueFamilyIndex(),
          glfw_window}
{
    if (!_vku_window.ok()) {
        std::cout << "Window creation failed" << std::endl;
        exit(1);
    }
}

} // namespace Cool

// #ifdef __COOL_APP_VULKAN
//     #include <imgui/backends/imgui_impl_vulkan.h>
//     #include "VulkanCheckResult.h"
// #endif

// namespace Cool {

// #ifdef __COOL_APP_VULKAN
// Window::Window(GLFWwindow* window, VulkanContext& vulkan_context)
//     : m_window(window), _vulkan_context(vulkan_context)
// {
// }

// Window::Window(Window&& o) noexcept
//     : m_window(o.m_window), _vulkan_window_state(o._vulkan_window_state), _vulkan_context(o._vulkan_context)
// {
//     o.m_window = nullptr;
// }
// #endif
// #ifdef __COOL_APP_OPENGL
// Window::Window(GLFWwindow* window)
//     : m_window(window)
// {
// }

// Window::Window(Window&& o) noexcept
//     : m_window(o.m_window)
// {
//     o.m_window = nullptr;
// }
// #endif

// void Window::destroy()
// {
// #ifdef __COOL_APP_VULKAN
//     ImGui_ImplVulkanH_DestroyWindow(_vulkan_context.g_Instance, _vulkan_context.g_Device, &_vulkan_window_state.g_MainWindowData, _vulkan_context.g_Allocator);
// #endif
//     if (m_window != nullptr) // Could have been moved
//         glfwDestroyWindow(m_window);
// }

// bool Window::checkForFullscreenToggles(int key, int scancode, int action, int mods)
// {
//     if (action == GLFW_RELEASE) {
//         if (key == GLFW_KEY_F11) {
//             switchFullScreen();
//             return true;
//         }
//         if (key == GLFW_KEY_ESCAPE && m_bIsFullScreen) {
//             escapeFullScreen();
//             return true;
//         }
//     }
//     return false;
// }

// void Window::switchFullScreen()
// {
//     if (m_bIsFullScreen)
//         escapeFullScreen();
//     else {
//         GLFWmonitor*       monitor = getCurrentMonitor();
//         const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
//         glfwGetWindowPos(m_window, &m_posXBeforeFullscreen, &m_posYBeforeFullscreen);
//         glfwGetWindowSize(m_window, &m_widthBeforeFullscreen, &m_heightBeforeFullscreen);
//         glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
//         if (m_bIsVSyncEnabled)
//             setSwapInterval(GLFW_TRUE);
//         m_bIsFullScreen = true;
//     }
// }

// void Window::escapeFullScreen()
// {
//     if (m_bIsFullScreen) {
//         GLFWmonitor*       monitor = getCurrentMonitor();
//         const GLFWvidmode* mode    = glfwGetVideoMode(monitor);
//         glfwSetWindowMonitor(m_window, NULL, m_posXBeforeFullscreen, m_posYBeforeFullscreen, m_widthBeforeFullscreen, m_heightBeforeFullscreen, mode->refreshRate);
//         if (m_bIsVSyncEnabled)
//             setSwapInterval(GLFW_TRUE);
//         m_bIsFullScreen = false;
//     }
// }

// void Window::enableVSync()
// {
//     setSwapInterval(GLFW_TRUE);
//     m_bIsVSyncEnabled = true;
// }

// void Window::disableVSync()
// {
//     setSwapInterval(GLFW_FALSE);
//     m_bIsVSyncEnabled = false;
// }

// void Window::setSwapInterval(int value)
// {
//     //GLFWwindow* currentWindow = glfwGetCurrentContext();
//     //glfwMakeContextCurrent(m_window);
//     //glfwSwapInterval(value);
//     //glfwMakeContextCurrent(currentWindow);
// }

// static int mini(int x, int y) { return x < y ? x : y; }
// static int maxi(int x, int y) { return x > y ? x : y; }

// GLFWmonitor* Window::getCurrentMonitor() const
// {
//     // Thanks to https://stackoverflow.com/questions/21421074/how-to-create-a-full-screen-window-on-the-current-monitor-with-glfw
//     int                nmonitors, i;
//     int                wx, wy, ww, wh;
//     int                mx, my, mw, mh;
//     int                overlap, bestoverlap;
//     GLFWmonitor*       bestmonitor;
//     GLFWmonitor**      monitors;
//     const GLFWvidmode* mode;

//     bestoverlap = 0;
//     bestmonitor = NULL;

//     glfwGetWindowPos(m_window, &wx, &wy);
//     glfwGetWindowSize(m_window, &ww, &wh);
//     monitors = glfwGetMonitors(&nmonitors);

//     for (i = 0; i < nmonitors; i++) {
//         mode = glfwGetVideoMode(monitors[i]);
//         glfwGetMonitorPos(monitors[i], &mx, &my);
//         mw = mode->width;
//         mh = mode->height;

//         overlap =
//             maxi(0, mini(wx + ww, mx + mw) - maxi(wx, mx)) *
//             maxi(0, mini(wy + wh, my + mh) - maxi(wy, my));

//         if (bestoverlap < overlap) {
//             bestoverlap = overlap;
//             bestmonitor = monitors[i];
//         }
//     }

//     return bestmonitor;
// }

// #ifdef __COOL_APP_VULKAN

// void Window::check_for_swapchain_rebuild()
// {
//     if (_vulkan_window_state.g_SwapChainRebuild) {
//         int width, height;
//         glfwGetFramebufferSize(m_window, &width, &height);
//         if (width > 0 && height > 0) {
//             ImGui_ImplVulkan_SetMinImageCount(_vulkan_window_state.g_MinImageCount);
//             ImGui_ImplVulkanH_CreateOrResizeWindow(_vulkan_context.g_Instance, _vulkan_context.g_PhysicalDevice, _vulkan_context.g_Device, &_vulkan_window_state.g_MainWindowData, _vulkan_context.g_QueueFamily, _vulkan_context.g_Allocator, width, height, _vulkan_window_state.g_MinImageCount);
//             _vulkan_window_state.g_MainWindowData.FrameIndex = 0;
//             _vulkan_window_state.g_SwapChainRebuild          = false;
//         }
//     }
// }

// void Window::FrameRender(ImDrawData* draw_data)
// {
//     auto*    wd = &_vulkan_window_state.g_MainWindowData;
//     VkResult err;

//     VkSemaphore image_acquired_semaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;
//     VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
//     err                                   = vkAcquireNextImageKHR(_vulkan_context.g_Device, wd->Swapchain, UINT64_MAX, image_acquired_semaphore, VK_NULL_HANDLE, &wd->FrameIndex);
//     if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
//         _vulkan_window_state.g_SwapChainRebuild = true;
//         return;
//     }
//     check_vk_result(err);

//     ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
//     {
//         err = vkWaitForFences(_vulkan_context.g_Device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking
//         check_vk_result(err);

//         err = vkResetFences(_vulkan_context.g_Device, 1, &fd->Fence);
//         check_vk_result(err);
//     }
//     {
//         err = vkResetCommandPool(_vulkan_context.g_Device, fd->CommandPool, 0);
//         check_vk_result(err);
//         VkCommandBufferBeginInfo info = {};
//         info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
//         info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
//         err = vkBeginCommandBuffer(fd->CommandBuffer, &info);
//         check_vk_result(err);
//     }
//     {
//         VkRenderPassBeginInfo info    = {};
//         info.sType                    = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//         info.renderPass               = wd->RenderPass;
//         info.framebuffer              = fd->Framebuffer;
//         info.renderArea.extent.width  = wd->Width;
//         info.renderArea.extent.height = wd->Height;
//         info.clearValueCount          = 1;
//         info.pClearValues             = &wd->ClearValue;
//         vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
//     }

//     // Record dear imgui primitives into command buffer
//     ImGui_ImplVulkan_RenderDrawData(draw_data, fd->CommandBuffer);

//     // Submit command buffer
//     vkCmdEndRenderPass(fd->CommandBuffer);
//     {
//         VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
//         VkSubmitInfo         info       = {};
//         info.sType                      = VK_STRUCTURE_TYPE_SUBMIT_INFO;
//         info.waitSemaphoreCount         = 1;
//         info.pWaitSemaphores            = &image_acquired_semaphore;
//         info.pWaitDstStageMask          = &wait_stage;
//         info.commandBufferCount         = 1;
//         info.pCommandBuffers            = &fd->CommandBuffer;
//         info.signalSemaphoreCount       = 1;
//         info.pSignalSemaphores          = &render_complete_semaphore;

//         err = vkEndCommandBuffer(fd->CommandBuffer);
//         check_vk_result(err);
//         err = vkQueueSubmit(_vulkan_context.g_Queue, 1, &info, fd->Fence);
//         check_vk_result(err);
//     }
// }

// void Window::FramePresent()
// {
//     auto* wd = &_vulkan_window_state.g_MainWindowData;
//     if (_vulkan_window_state.g_SwapChainRebuild)
//         return;
//     VkSemaphore      render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
//     VkPresentInfoKHR info                      = {};
//     info.sType                                 = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
//     info.waitSemaphoreCount                    = 1;
//     info.pWaitSemaphores                       = &render_complete_semaphore;
//     info.swapchainCount                        = 1;
//     info.pSwapchains                           = &wd->Swapchain;
//     info.pImageIndices                         = &wd->FrameIndex;
//     VkResult err                               = vkQueuePresentKHR(_vulkan_context.g_Queue, &info);
//     if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
//         _vulkan_window_state.g_SwapChainRebuild = true;
//         return;
//     }
//     check_vk_result(err);
//     wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
// }

// #endif

// } // namespace Cool