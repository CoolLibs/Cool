#pragma once

#include "MessageConsole.h"

namespace Cool::Log::Debug {

/// Displays a green message in the debug console.
/// Does nothing in release mode.
void info(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

/// Displays a yellow message in the debug console.
/// Does nothing in release mode.
void warning(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

/// Displays a red message in the debug console and triggers a breakpoint.
/// Does nothing in release mode.
void error(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

/// Displays a red message in the debug console.
/// Does nothing in release mode.
void error_without_breakpoint(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

#if DEBUG
auto console() -> MessageConsole&;
#endif

} // namespace Cool::Log::Debug