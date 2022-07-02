#include "MessageConsole.h"
#include <Cool/Constants/Constants.h>
#include <stringify/stringify.hpp>

namespace Cool {

void MessageConsole::clear(const MessageId& id)
{
    _messages.destroy(id);
}

void MessageConsole::send(MessageId& id, const MessageV2& message)
{
    if (id.underlying_uuid().is_nil() ||
        !_messages.contains(id))
    {
        id = _messages.create({
            .message   = message,
            .timestamp = std::chrono::system_clock::now(),
            .count     = 0,
        });
    }
    else
    {
        _messages.with_mutable_ref(id, [&](internal::MessageWithMetadata& data) {
            data.message = message;
            data.count++;
            data.timestamp = std::chrono::system_clock::now();
        });
    }
}

void MessageConsole::imgui()
{
    _selected_message = MessageId{};
    for (const auto& [id, msg] : _messages)
    {
        ImGui::BeginGroup();

        ImGui::TextColored(
            Cool::Constants::imvec4_red,
            "[%s] [#%lld] [%s]",
            Cool::stringify(msg.timestamp).c_str(),
            msg.count,
            msg.message.category.c_str()
        );
        ImGui::SameLine();
        ImGui::Text("%s", msg.message.detailed_message.c_str());

        ImGui::EndGroup();
        if (ImGui::IsItemHovered())
        {
            _selected_message = id;
        }
    }
}

auto MessageConsole::should_highlight(const MessageId& id) -> bool
{
    return !id.underlying_uuid().is_nil() &&
           id == _selected_message;
}

} // namespace Cool
