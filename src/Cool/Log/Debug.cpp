#include "Debug.h"

namespace Cool::Log::Debug {

void info(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    console().send(Message{
        .category           = category,
        .message            = message,
        .severity           = MessageSeverity::Info,
        .clipboard_contents = clipboard_contents,
    });
#else
    (void)category;
    (void)message;
    (void)clipboard_contents;
#endif
}

void warning(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    console().send(Message{
        .category           = category,
        .message            = message,
        .severity           = MessageSeverity::Warning,
        .clipboard_contents = clipboard_contents,
    });
#else
    (void)category;
    (void)message;
    (void)clipboard_contents;
#endif
}

void error(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    error_without_breakpoint(category, message, clipboard_contents);
    assert(false);
#else
    (void)category;
    (void)message;
    (void)clipboard_contents;
#endif
}

void error_without_breakpoint(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    console().send(Message{
        .category           = category,
        .message            = message,
        .severity           = MessageSeverity::Error,
        .clipboard_contents = clipboard_contents,
    });
#else
    (void)category;
    (void)message;
    (void)clipboard_contents;
#endif
}

#if DEBUG
auto console() -> MessageConsole&
{
    static auto the_console = MessageConsole{"Debug Console"};
    return the_console;
}
#endif

} // namespace Cool::Log::Debug
