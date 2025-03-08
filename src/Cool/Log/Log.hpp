#pragma once
#include "ClipboardContent.hpp"

namespace Cool::Log {

void info(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents = {});
void warning(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents = {});
void error(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents = {});

void internal_info(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents = {}, std::optional<bool> should_log_to_user = std::nullopt);
void internal_warning(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents = {}, std::optional<bool> should_log_to_user = std::nullopt);
void internal_error(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents = {}, std::optional<bool> should_log_to_user = std::nullopt);

} // namespace Cool::Log