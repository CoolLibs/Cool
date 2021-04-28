#pragma once

#include <vulkan/vulkan.h>

namespace Cool {

struct VulkanContext {
	VulkanContext(const char** extensions, uint32_t extensions_count);
	~VulkanContext() = default;
	void destroy0();
	void destroy1();

	VkAllocationCallbacks*   g_Allocator = NULL;
	VkInstance               g_Instance = VK_NULL_HANDLE;
	VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
	VkDevice                 g_Device = VK_NULL_HANDLE;
	uint32_t                 g_QueueFamily = (uint32_t)-1;
	VkQueue                  g_Queue = VK_NULL_HANDLE;
	VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
	VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
	VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;
};

}  // namespace Cool