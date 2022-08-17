#pragma once

#include "OptionalErrorMessage.h"

namespace Cool {

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

} // namespace Cool