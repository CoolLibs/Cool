#pragma once

#include "Message.h"

namespace Cool::internal {

struct MessageWithMetadata {
    Message                               message;
    std::chrono::system_clock::time_point timestamp;
    uint32_t                              count;
    bool                                  forced_to_be_closable = false;
};

} // namespace Cool::internal
