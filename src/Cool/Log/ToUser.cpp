#include "ToUser.h"

namespace Cool::Log::ToUser {

void info(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
    console().send(Message{
        .category           = category,
        .message            = message,
        .severity           = MessageSeverity::Info,
        .clipboard_contents = clipboard_contents,
    });
}

void warning(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
    console().send(Message{
        .category           = category,
        .message            = message,
        .severity           = MessageSeverity::Warning,
        .clipboard_contents = clipboard_contents,
    });
}

void error(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
    console().send(Message{
        .category           = category,
        .message            = message,
        .severity           = MessageSeverity::Error,
        .clipboard_contents = clipboard_contents,
    });
}

auto console() -> MessageConsole&
{
    static auto the_console = MessageConsole{"Console"};
    return the_console;
}

} // namespace Cool::Log::ToUser
