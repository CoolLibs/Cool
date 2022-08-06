#pragma once

#include "internal/MessageConsole.h"

namespace Cool::Log::Debug {

/// Displays a green message in the debug console.
/// Does nothing in release mode.
void info(const std::string& category, const std::string& message);

/// Displays a yellow message in the debug console.
/// Does nothing in release mode.
void warning(const std::string& category, const std::string& message);

/// Displays a red message in the debug console and triggers a breakpoint.
/// Does nothing in release mode.
void error(const std::string& category, const std::string& message);

/// Displays a red message in the debug console.
/// Does nothing in release mode.
void error_without_breakpoint(const std::string& category, const std::string& message);

#if DEBUG
auto console() -> MessageConsole&;
#endif

} // namespace Cool::Log::Debug