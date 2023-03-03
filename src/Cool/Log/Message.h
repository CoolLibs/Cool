#pragma once

namespace Cool {

enum class MessageSeverity {
    Info,
    Warning,
    Error,
};

struct ClipboardContent {
    std::string title{};
    std::string content{};
};

struct Message {
    std::string                                  category;
    std::string                                  message;
    MessageSeverity                              severity{MessageSeverity::Error};
    std::optional<std::vector<ClipboardContent>> clipboard_contents{}; // Controls the string that will be copied to the clipboard when users click on the message. If left empty, the whole `message` will be copied to the clipboard.
};

} // namespace Cool