#pragma once

namespace Cool {

enum class MessageSeverity {
    Info,
    Warning,
    Error,
};

struct MessageV2 { // TODO rename as Message once the old ToUser system is gone
    std::string     category;
    std::string     detailed_message;
    MessageSeverity severity{MessageSeverity::Error};
};

} // namespace Cool