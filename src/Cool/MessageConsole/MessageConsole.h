#pragma once

#include <reg/reg.hpp>
#include "Message.h"
#include "MessageId.h"

namespace Cool {

class MessageConsole {
public:
    /// Removes the message from the list, because it was resolved.
    void clear(const MessageId& id);
    // void clear_all(); // TODO I don't think this is usefull, errors will pop again immediately, or they have been closed and would disappear automatically anyways
    /// If id is null, creates a new error and sets id to it, otherwise refresh the error referenced by id.
    void send(MessageId&, const MessageV2&);

    void imgui(); // TODO should focus associated window wehn clicking (or hovering) on an error

    /// Returns true iff the error is currently selected / hovered by the user
    /// Allows us to focus the corresponding window
    auto should_highlight(const MessageId&) -> bool;

private:
    reg::Registry<internal::MessageWithMetadata> _messages; // TODO(JF) Use an order-preserving registry
    MessageId                                    _selected_message;
};

} // namespace Cool