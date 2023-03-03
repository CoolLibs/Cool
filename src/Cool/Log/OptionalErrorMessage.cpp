#include "OptionalErrorMessage.h"
#include "Cool/Log/OptionalErrorMessage.h"

namespace Cool {

auto OptionalErrorMessage::gen_message(std::function<Cool::Message(const std::string&)> const& transform) const -> Message
{
    auto message = transform(*_message);
    assert(!message.clipboard_contents); // The clipboard contents are already stored in the OptionalErrorMessage class and will overwrite any that might have been set by the `transform`.
    message.clipboard_contents = _clipboard_contents;
    return message;
}

void OptionalErrorMessage::send_error_if_any(
    MessageSender&                                          sender,
    std::function<Cool::Message(const std::string&)> const& transform
) const
{
    if (_message)
    {
        sender.send(gen_message(transform));
    }
    else
    {
        sender.clear();
    }
}

void OptionalErrorMessage::send_error_if_any(
    MessageId&                                              id,
    std::function<Cool::Message(const std::string&)> const& transform,
    MessageConsole&                                         console
) const
{
    if (_message)
    {
        console.send(id, gen_message(transform));
    }
    else
    {
        console.remove(id);
    }
}

void OptionalErrorMessage::send_error_if_any(
    std::function<Cool::Message(const std::string&)> const& transform,
    MessageConsole&                                         console
) const
{
    if (_message)
    {
        console.send(gen_message(transform));
    }
}

} // namespace Cool