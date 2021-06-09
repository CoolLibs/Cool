#pragma once
#ifdef __COOL_APP_VULKAN

#include <vulkan/vulkan.h>

namespace Cool {

inline void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    Log::error("[vulkan] Error: VkResult = {}", err);
    if (err < 0)
        abort();
}

} // namespace Cool

#endif