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

class Window_Vulkan : public Window_Base {
public:
    Window_Vulkan(Window_Vulkan&&) noexcept = default;
    Window_Vulkan& operator=(Window_Vulkan&&) noexcept = default;
    Window_Vulkan::~Window_Vulkan();

    void check_for_swapchain_rebuild();
    void rebuild_swapchain();
    void FramePresent();
    void FrameRender(ImDrawData* draw_data, std::function<void(vk::CommandBuffer)> render_fn);

    void cap_framerate(bool should_cap) override;
    bool framerate_is_capped() const override;

private:
    // To construct a window, use WindowFactory_Vulkan::make_window()
    friend class WindowFactory_Vulkan;
    Window_Vulkan(GLFWwindow* window);

public:
    VulkanWindowState _vulkan_window_state;

private:
    bool _present_mode_mailbox_is_avaible = false;
};

} // namespace Cool

#endif