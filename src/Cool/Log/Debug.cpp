#pragma once

#include "Debug.h"

namespace Cool::Log::Debug {

void info(const std::string& category, const std::string& message) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    console().send(MessageV2{
        .category         = category,
        .detailed_message = message,
        .severity         = MessageSeverity::Info,
    });
#endif
}

void warning(const std::string& category, const std::string& message) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    console().send(MessageV2{
        .category         = category,
        .detailed_message = message,
        .severity         = MessageSeverity::Warning,
    });
#endif
}

void error(const std::string& category, const std::string& message) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    error_without_breakpoint(category, message);
    assert(false);
#endif
}

void error_without_breakpoint(const std::string& category, const std::string& message) // We take string& instead of string_view because Message needs strings anyways.
{
#if DEBUG
    console().send(MessageV2{
        .category         = category,
        .detailed_message = message,
        .severity         = MessageSeverity::Error,
    });
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
