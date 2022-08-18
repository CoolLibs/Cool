#pragma once

#include "Message.h"
#include "MessageConsole.h"
#include "MessageId.h"

namespace Cool {

/// Allows you to send a message to a given `console`, update it, and clear it.
/// When the `MessageSender` is destroyed, the corresponding message is removed from the console.
/// (NB: this requires the `MessageConsole` to have a stable address in memory. For more information, read the documentation of `reg::ScopedId_Ordered`.)
class MessageSender {
public:
    explicit MessageSender(MessageConsole& console)
        : _console{console}
    {}

    void send(const Message& message) { _console.get().send(_id, message); }
    void clear() { _console.get().clear(_id); }
    auto should_highlight() const -> bool { return _console.get().should_highlight(_id); }

private:
    MessageId                              _id{};
    std::reference_wrapper<MessageConsole> _console;
};

} // namespace Cool