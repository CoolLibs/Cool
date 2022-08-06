#pragma once

#include <reg/reg.hpp>
#include "../MessageId.h"
#include "Message.h"


namespace Cool {

namespace internal {
/// Like MessageId, but does not own any message: the message is not destroyed when RawMessageId is destroyed.
using RawMessageId = reg::Id<internal::MessageWithMetadata>;
} // namespace internal

class MessageConsole {
public:
    explicit MessageConsole(const char* name = "Console")
        : _name{name}
    {}

    /// If `id` is null, creates a new messages and sets `id` to reference it,
    /// otherwise refreshes the message referenced by `id`.
    void send(MessageId&, const Message&);

    /// Sends a message to the console. No `id` is returned. The message can only be cleared by the user.
    /// Such a message will always be clearable by the user.
    void send(const Message&);

    /// Removes the message from the list.
    void clear(const MessageId& id);

    /// Removes all the closable messages from the list.
    void clear_all();

    /// Returns true iff the message is currently selected / hovered by the user.
    /// Allows us to focus the corresponding window / highlight the corresponding part of the UI that requires attention.
    auto should_highlight(const MessageId&) -> bool;

    /// Draws the imgui window with all the messages.
    void imgui_window();

private:
    void clear(const internal::RawMessageId& id);
    void close_window();
    void on_message_sent(const internal::RawMessageId&);
    void show_number_of_messages_of_given_severity(MessageSeverity);
    void refresh_counts_per_severity();
    void imgui_menu_bar();
    void imgui_show_all_messages();

    class MessagesCountPerSeverity {
    public:
        MessagesCountPerSeverity();
        void increment(MessageSeverity);
        void reset_to_zero();
        auto get(MessageSeverity) const -> size_t;

    private:
        std::array<size_t, 3> _counts_per_severity{};
    };

private:
    reg::OrderedRegistry<internal::MessageWithMetadata> _messages;
    internal::RawMessageId                              _selected_message;
    bool                                                _is_open{false};
    std::optional<internal::RawMessageId>               _message_just_sent{};
    const char*                                         _name;
    MessagesCountPerSeverity                            _counts_per_severity{};
};

} // namespace Cool