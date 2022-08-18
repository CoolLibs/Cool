#pragma once

#include "OptionalErrorMessage.h"

namespace Cool {

void OptionalErrorMessage::send_error_if_any(
    MessageSender&                                   sender,
    std::function<Cool::Message(const std::string&)> transform
) const
{
    if (_message)
    {
        sender.send(transform(*_message));
    }
    else
    {
        sender.clear();
    }
}

void OptionalErrorMessage::send_error_if_any(
    MessageId&                                       id,
    std::function<Cool::Message(const std::string&)> transform,
    MessageConsole&                                  console
) const
{
    if (_message)
    {
        console.send(id, transform(*_message));
    }
    else
    {
        console.clear(id);
    }
}

void OptionalErrorMessage::send_error_if_any(
    std::function<Cool::Message(const std::string&)> transform,
    MessageConsole&                                  console
) const
{
    if (_message)
    {
        console.send(transform(*_message));
    }
}

} // namespace Cool