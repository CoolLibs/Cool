#pragma once

#include "Message.h"

namespace Cool::internal {

struct MessageWithMetadata {
    MessageV2                             message;
    std::chrono::system_clock::time_point timestamp;
    uint64_t                              count;
};

} // namespace Cool::internal