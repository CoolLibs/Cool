#pragma once

#include <reg/reg.hpp>
#include "Message.h"
#include "MessageId.h"

namespace Cool {

class MessageConsole {
public:
    /// If `id` is null, creates a new messages and sets `id` to reference it,
    /// otherwise refreshes the message referenced by `id`.
    void send(MessageId&, const MessageV2&);

    /// Removes the message from the list.
    void clear(const MessageId& id);

    /// Returns true iff the message is currently selected / hovered by the user
    /// Allows us to focus the corresponding window / highlight the corresponding part of the UI that requires attention.
    auto should_highlight(const MessageId&) -> bool;

    /// Draws the imgui window with all the messages.
    void imgui_window();

    auto is_open() -> bool& { return _is_open; }

private:
    reg::Registry<internal::MessageWithMetadata> _messages; // TODO(JF) Use an order-preserving registry
    MessageId                                    _selected_message;
    bool                                         _is_open{false};
    bool                                         _scroll_to_bottom{false};
};

} // namespace Cool