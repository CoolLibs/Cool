#pragma once

#include "../Message.h"

namespace Cool::internal {

struct MessageWithMetadata {
    Message                               message;
    bool                                  forced_to_be_clearable = false;
    std::chrono::system_clock::time_point timestamp              = std::chrono::system_clock::now();
    std::chrono::steady_clock::time_point monotonic_timestamp    = std::chrono::steady_clock::now();
    uint32_t                              count                  = 0;
};

} // namespace Cool::internal
