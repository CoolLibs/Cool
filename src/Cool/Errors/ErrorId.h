#pragma once

#include <reg/reg.hpp>
#include "Error.h"

namespace Cool {

namespace internal {

struct ErrorWithMetadata{
    Error                                 error;
    std::chrono::system_clock::time_point timestamp;
    size_t                                count;
};

} // namespace internal

using ErrorId = reg::Id<internal::ErrorWithMetadata>;

} // namespace Cool