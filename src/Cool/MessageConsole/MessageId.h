#pragma once

#include <reg/reg.hpp>
#include "Message.h"

namespace Cool {

namespace internal {

struct MessageWithMetadata {
    MessageV2                             message;
    std::chrono::system_clock::time_point timestamp;
    size_t                                count;
};

} // namespace internal

using MessageId = reg::Id<internal::MessageWithMetadata>;

} // namespace Cool