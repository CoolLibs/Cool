#pragma once

#include <reg/reg.hpp>
#include "MessageWithMetadata.h"

namespace Cool {

using MessageId         = reg::ScopedId_Ordered<internal::MessageWithMetadata>;
using UnscopedMessageId = reg::Id<internal::MessageWithMetadata>;

} // namespace Cool