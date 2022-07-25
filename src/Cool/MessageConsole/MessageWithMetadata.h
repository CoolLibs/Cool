#pragma once

#include "Message.h"

namespace Cool::internal {

struct MessageWithMetadata {
    MessageV2                             message;
    std::chrono::system_clock::time_point timestamp;
    uint32_t                              count;
};

} // namespace Cool::internal