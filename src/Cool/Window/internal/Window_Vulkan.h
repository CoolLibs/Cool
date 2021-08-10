#pragma once
#if defined(__COOL_APP_VULKAN)

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h> // TODO remove me when VulkanWindowState stops using ImGui_ImplVulkanH_Window
#include "Window_Base.h"

namespace Cool {

struct VulkanWindowState {
    ImGui_ImplVulkanH_Window g_MainWindowData;
    int                      g_MinImageCount    = 2;
    bool                     g_SwapChainRebuild = false;
};

class Window_Vulkan {
public:
    Window_Vulkan(GLFWwindow* window);
    Window_Vulkan(Window_Vulkan&&) noexcept = default;
    Window_Vulkan& operator=(Window_Vulkan&&) noexcept = default;
    Window_Vulkan(const Window_Vulkan&)                = delete; // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
    Window_Vulkan& operator=(const Window_Vulkan&) = delete;     // Non-copyable because there should only be ONE owner of the window. Please store references to the window if you need to.
    void           destroy();

    void check_for_swapchain_rebuild();
    void FramePresent();
    void FrameRender(ImDrawData* draw_data, std::function<void(vk::CommandBuffer)> render_fn);

    Window_Base& operator*() { return _base; }
    Window_Base* operator->() { return &operator*(); }

public:
    VulkanWindowState _vulkan_window_state;

private:
    Window_Base _base;
};

} // namespace Cool

#endif