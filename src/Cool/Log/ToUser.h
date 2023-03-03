#pragma once

#include "MessageConsole.h"

namespace Cool::Log::ToUser {

/// Displays a green message in the user-facing console.
void info(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

/// Displays a yellow message in the user-facing console.
void warning(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

/// Displays a red message in the user-facing console.
void error(std::string const& category, std::string const& message, std::optional<std::vector<ClipboardContent>> const& clipboard_contents = {});

auto console() -> MessageConsole&;

} // namespace Cool::Log::ToUser