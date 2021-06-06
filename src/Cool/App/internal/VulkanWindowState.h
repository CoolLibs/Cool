#pragma once
#ifdef __COOL_APP_VULKAN

#include <imgui/backends/imgui_impl_vulkan.h>

namespace Cool {

struct VulkanWindowState {
	ImGui_ImplVulkanH_Window g_MainWindowData;
	int  g_MinImageCount = 2;
	bool g_SwapChainRebuild = false;
};

}  // namespace Cool

#endif