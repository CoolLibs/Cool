#pragma once
#if defined(COOL_VULKAN)

#include <vulkan/vulkan.h>

namespace Cool::Vulkan {

inline void check_result(VkResult err)
{
    if (err == 0)
        return;
    Log::Debug::error("[vulkan] Error: VkResult = {}", err);
    if (err < 0)
        abort();
}

} // namespace Cool::Vulkan

#endif