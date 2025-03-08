#pragma once
#include "MessageConsole.h"
#include "MessageId.h"
#include "message_console.hpp"

namespace Cool {

class TestMessageConsole {
public:
    TestMessageConsole();

    void imgui();

private:
    struct Message {
        std::string           category{"Test"};
        std::string           message{"Hello!"};
        Cool::MessageSeverity severity{Cool::MessageSeverity::Error};
        Cool::MessageId       id{};

        void send()
        {
            message_console().send(
                id,
                {
                    .category = category,
                    .message  = message,
                    .severity = severity,
                }
            );
        }
    };

    void imgui(Message& message);

private:
    std::vector<Message>     _messages{};
    size_t                   _next_message_number{0};
    std::optional<MessageId> _scoped_message_id{};
};

} // namespace Cool
