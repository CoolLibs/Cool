#pragma once

#include "Cool/Log/Message.h"
#include "MessageConsole.h"
#include "MessageSender.h"
#include "ToUser.h"

namespace Cool {

class [[nodiscard]] OptionalErrorMessage {
public:
    OptionalErrorMessage() = default;
    OptionalErrorMessage(std::string_view message, std::optional<std::vector<ClipboardContent>> clipboard_contents = {}) // NOLINT(*-explicit-constructor, *-explicit-conversions)
        : _message{message}
        , _clipboard_contents{std::move(clipboard_contents)}
    {}

    operator bool() const { return static_cast<bool>(_message); } // NOLINT(*-explicit-constructor, *-explicit-conversions)

    /// Creates an error message by applying `transform` to a string, then sends it.
    /// Does nothing if there was no error message in the first place.
    void send_error_if_any(
        MessageSender&                                          sender,
        std::function<Cool::Message(const std::string&)> const& transform
    ) const;

    /// Creates an error message by applying `transform` to a string, then sends it.
    /// Does nothing if there was no error message in the first place.
    void send_error_if_any(
        MessageId&                                              id,
        std::function<Cool::Message(const std::string&)> const& transform,
        MessageConsole&                                         console = Log::ToUser::console()
    ) const;

    /// Creates an error message by applying `transform` to a string, then sends it.
    /// Does nothing if there was no error message in the first place.
    void send_error_if_any(
        std::function<Cool::Message(const std::string&)> const& transform,
        MessageConsole&                                         console = Log::ToUser::console()
    ) const;

private:
    auto gen_message(std::function<Cool::Message(const std::string&)> const& transform) const -> Message;

private:
    std::optional<std::string>                   _message{};
    std::optional<std::vector<ClipboardContent>> _clipboard_contents{};
};

} // namespace Cool