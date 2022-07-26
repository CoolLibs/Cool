#pragma once

#include <reg/reg.hpp>
#include "Message.h"
#include "MessageId.h"

namespace Cool {

class MessageConsole {
public:
    explicit MessageConsole(const char* name = "Console")
        : _name{name}
    {}

    /// If `id` is null, creates a new messages and sets `id` to reference it,
    /// otherwise refreshes the message referenced by `id`.
    void send(MessageId&, const MessageV2&);

    /// Sends a message to the console. No `id` is returned. The message can only be cleared by the user.
    /// Such a message will always be clearable by the user.
    void send(const MessageV2&);

    /// Removes the message from the list.
    void clear(const MessageId& id);

    /// Returns true iff the message is currently selected / hovered by the user.
    /// Allows us to focus the corresponding window / highlight the corresponding part of the UI that requires attention.
    auto should_highlight(const MessageId&) -> bool;

    /// Draws the imgui window with all the messages.
    void imgui_window();

    auto is_open() -> bool& { return _is_open; }

private:
    void close_window();
    void on_message_sent(const MessageId&);

private:
    reg::OrderedRegistry<internal::MessageWithMetadata> _messages;
    MessageId                                           _selected_message;
    bool                                                _is_open{false};
    std::optional<MessageId>                            _message_just_sent{};
    const char*                                         _name;
};

} // namespace Cool