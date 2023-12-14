#include "Exception.h"
#include "Cool/Log/OptionalErrorMessage.h"

namespace Cool {

Exception::Exception(OptionalErrorMessage message)
    : _error_message{std::move(message)}
{}

auto Exception::what() const -> const char*
{
    auto const& msg = _error_message.error_message();
    if (msg.has_value())
        return msg.value().c_str();
    return "NO ERROR"; // This shouldn't happen
}

auto Exception::error_message() const -> OptionalErrorMessage const&
{
    return _error_message;
}

} // namespace Cool