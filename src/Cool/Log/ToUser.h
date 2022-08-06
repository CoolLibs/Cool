#pragma once

#include "MessageConsole.h"

namespace Cool::Log::ToUser {

/// Displays a green message in the user-facing console.
void info(const std::string& category, const std::string& message);

/// Displays a yellow message in the user-facing console.
void warning(const std::string& category, const std::string& message);

/// Displays a red message in the user-facing console.
void error(const std::string& category, const std::string& message);

auto console() -> MessageConsole&;

} // namespace Cool::Log::ToUser