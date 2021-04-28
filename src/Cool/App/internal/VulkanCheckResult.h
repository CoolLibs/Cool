#pragma once

#include <vulkan/vulkan.h>

namespace Cool {

inline void check_vk_result(VkResult err)
{
    if (err == 0)
        return;
    Log::Error("[vulkan] Error: VkResult = {}", err);
    if (err < 0)
        abort();
}

} // namespace Cool