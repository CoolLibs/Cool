#pragma once
#if defined(__COOL_APP_VULKAN)

#include <vulkan/vulkan.h>

namespace Cool::Vulkan {

inline void check_result(VkResult err)
{
    if (err == 0)
        return;
    Log::error("[vulkan] Error: VkResult = {}", err);
    if (err < 0)
        abort();
}

} // namespace Cool::Vulkan

#endif