#pragma once
#if defined(COOL_VULKAN)

#include <Vulkan/Vulkan.h>

namespace Cool::Vulkan {

struct Context {
    Context();
    void destroy0();
    void destroy1();

    VkAllocationCallbacks*           g_Allocator      = NULL;
    VkInstance                       g_Instance       = VK_NULL_HANDLE;
    VkPhysicalDevice                 g_PhysicalDevice = VK_NULL_HANDLE;
    VkDevice                         g_Device         = VK_NULL_HANDLE;
    uint32_t                         g_QueueFamily    = (uint32_t)-1;
    VkQueue                          g_Queue          = VK_NULL_HANDLE;
    VkDebugReportCallbackEXT         g_DebugReport    = VK_NULL_HANDLE;
    VkPipelineCache                  g_PipelineCache  = VK_NULL_HANDLE;
    VkDescriptorPool                 g_DescriptorPool = VK_NULL_HANDLE;
    VkCommandPool                    command_pool     = VK_NULL_HANDLE;
    VkPhysicalDeviceMemoryProperties memory_properties;
};

inline Context& context()
{
    static Context ctx;
    return ctx;
}

} // namespace Cool::Vulkan

#endif