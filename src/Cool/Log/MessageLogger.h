#pragma once

#include "Message.h"
#include "MessageId.h"

namespace Cool {

/// Wraps a MessageId and sends messages to the user-facing console, or clears the previous message if there is no message.
class MessageLogger {
public:
    void handle(const std::optional<Message>&);

private:
    MessageId _id;
};

} // namespace Cool