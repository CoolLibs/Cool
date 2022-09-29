#pragma once

namespace Cool {

enum class MessageSeverity {
    Info,
    Warning,
    Error,
};

struct Message {
    std::string     category;
    std::string     message;
    MessageSeverity severity{MessageSeverity::Error};
};

} // namespace Cool