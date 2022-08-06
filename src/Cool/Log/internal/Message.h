#pragma once

namespace Cool {

enum class MessageSeverity {
    Info,
    Warning,
    Error,
};

struct Message {
    std::string     category;
    std::string     detailed_message;
    MessageSeverity severity{MessageSeverity::Error};
};

} // namespace Cool