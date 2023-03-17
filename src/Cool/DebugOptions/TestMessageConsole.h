#pragma once
#if DEBUG

#include <Cool/Log/MessageConsole.h>
#include "Cool/Log/MessageId.h"

namespace Cool {

class TestMessageConsole {
public:
    TestMessageConsole();

    void imgui(Cool::MessageConsole& message_console);

private:
    struct Message {
        std::string           category{"Test"};
        std::string           message{"Hello!"};
        Cool::MessageSeverity severity{Cool::MessageSeverity::Error};
        Cool::MessageId       id{};

        void send_to(Cool::MessageConsole& message_console)
        {
            message_console.send(id, {
                                         .category = category,
                                         .message  = message,
                                         .severity = severity,
                                     });
        }
    };

    void imgui(Message& message, Cool::MessageConsole& message_console);

private:
    std::vector<Message>     _messages{};
    size_t                   _next_message_number{0};
    std::optional<MessageId> _scoped_message_id{};
};

} // namespace Cool

#endif