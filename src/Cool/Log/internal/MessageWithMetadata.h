#pragma once

#include "Message.h"

namespace Cool::internal {

struct MessageWithMetadata {
    Message                               message;
    bool                                  forced_to_be_closable = false;
    std::chrono::system_clock::time_point timestamp             = std::chrono::system_clock::now();
    uint32_t                              count                 = 0;
};

} // namespace Cool::internal
