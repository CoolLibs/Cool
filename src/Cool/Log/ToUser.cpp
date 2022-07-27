#pragma once

#include "ToUser.h"

namespace Cool::Log::ToUser {

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
    console().send(MessageV2{
        .category         = category,
        .detailed_message = message,
        .severity         = MessageSeverity::Error,
    });
#endif
}

auto console() -> MessageConsole&
{
    static auto the_console = MessageConsole{"Console"};
    return the_console;
}

} // namespace Cool::Log::ToUser
