#pragma once
#include "ClipboardContent.hpp"

namespace Cool {

enum class MessageSeverity : uint8_t {
    Info,
    Warning,
    Error,
};

struct Message {
    std::string                   category;
    std::string                   message;
    MessageSeverity               severity{MessageSeverity::Error};
    std::vector<ClipboardContent> clipboard_contents{}; // Controls the string that will be copied to the clipboard when users click on the message. If left empty, the whole `message` will be copied to the clipboard.
};

} // namespace Cool