#pragma once
#if defined(COOL_VULKAN)

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <imgui/backends/imgui_impl_vulkan.h> // TODO(JF) remove me when VulkanWindowState stops using ImGui_ImplVulkanH_Window
#include "Window_Base.h"

namespace Cool {

struct VulkanWindowState {
    ImGui_ImplVulkanH_Window g_MainWindowData;
    int                      g_MinImageCount    = 2;
    bool                     g_SwapChainRebuild = false;
};

class Window_Vulkan {
public:
    ~Window_Vulkan();

    void check_for_swapchain_rebuild();
    void rebuild_swapchain();
    void FramePresent();
    void FrameRender(ImDrawData* draw_data);

    void cap_framerate_if(bool should_cap) override;
    bool framerate_is_capped() const override;

public:
    VulkanWindowState _vulkan_window_state;

private:
    friend class WindowFactory_ImplVulkan;
    bool _present_mode_mailbox_is_avaible = false;
};

} // namespace Cool

#endif