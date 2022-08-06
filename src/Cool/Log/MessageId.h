#pragma once

#include <reg/reg.hpp>
#include "internal/MessageWithMetadata.h"

namespace Cool {

/// Allows you to send a message to the console, update it, and remove it.
/// When the `MessageId` is destroyed, the corresponding message is removed from the console.
/// (NB: this requires the `MessageConsole` to have a stable address in memory. For more information, read the documentation of `reg::ScopedId_Ordered`.)
using MessageId = reg::ScopedId_Ordered<internal::MessageWithMetadata>;

} // namespace Cool