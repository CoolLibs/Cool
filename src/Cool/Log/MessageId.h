#pragma once

#include <reg/reg.hpp>
#include "internal/MessageWithMetadata.h"

namespace Cool {

/// Allows you to send a message to the console, update it, and remove it.
/// When the `MessageId` is destroyed, the corresponding message is removed from the console.
using MessageId = reg::SharedId<internal::MessageWithMetadata>; // TODO(Variables) Make it a UniqueId again

} // namespace Cool