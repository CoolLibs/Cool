#pragma once

#include "MessageConsole.h"
#include "MessageSender.h"
#include "ToUser.h"

namespace Cool {

class [[nodiscard]] OptionalErrorMessage {
public:
    OptionalErrorMessage() = default;
    OptionalErrorMessage(std::string_view message) // We want this constructor to be implicit.
        : _message{message}
    {}

         operator bool() const { return static_cast<bool>(_message); }
    auto operator*() const -> std::string { return *_message; }

    /// Creates an error message by applying `transform` to a string, then sends it.
    /// Does nothing if there was no error message in the first place.
    void send_error_if_any(
        MessageSender&                                   sender,
        std::function<Cool::Message(const std::string&)> transform
    ) const;

    /// Creates an error message by applying `transform` to a string, then sends it.
    /// Does nothing if there was no error message in the first place.
    void send_error_if_any(
        MessageId&                                       id,
        std::function<Cool::Message(const std::string&)> transform,
        MessageConsole&                                  console = Log::ToUser::console()
    ) const;

    /// Creates an error message by applying `transform` to a string, then sends it.
    /// Does nothing if there was no error message in the first place.
    void send_error_if_any(
        std::function<Cool::Message(const std::string&)> transform,
        MessageConsole&                                  console = Log::ToUser::console()
    ) const;

private:
    std::optional<std::string> _message{};
};

} // namespace Cool