#include "MessageConsole.h"
#include <Cool/Constants/Constants.h>
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <stringify/stringify.hpp>

namespace Cool {

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

    _is_open           = true;
    _message_just_sent = id;
}

void MessageConsole::clear(const MessageId& id)
{
    if (_messages.contains(id))
    {
        _messages.destroy(id);

        if (_messages.is_empty())
        {
            close_window();
        }
    }
}

void MessageConsole::close_window()
{
    _is_open          = false;
    _selected_message = MessageId{};
}

auto MessageConsole::should_highlight(const MessageId& id) -> bool
{
    return !id.underlying_uuid().is_nil() &&
           id == _selected_message;
}

static auto color(MessageSeverity severity) -> ImVec4
{
    switch (severity)
    {
    case MessageSeverity::Info:
        return Constants::imvec4_green;
    case MessageSeverity::Warning:
        return Constants::imvec4_yellow;
    case MessageSeverity::Error:
        return Constants::imvec4_red;
    default:
        Log::error("[MessageConsole::color] Unknown enum value");
        return ImVec4{0, 0, 0, 0};
    }
}

static auto to_string(MessageSeverity severity) -> std::string
{
    switch (severity)
    {
    case MessageSeverity::Info:
        return "info";
    case MessageSeverity::Warning:
        return "warning";
    case MessageSeverity::Error:
        return "error";
    default:
        Log::error("[MessageConsole::to_string] Unknown enum value");
        return "";
    }
}

static auto is_closable(const internal::MessageWithMetadata& msg) -> bool
{
    return msg.message.severity != MessageSeverity::Error;
}

void MessageConsole::imgui_window()
{
    if (_is_open)
    {
        if (_message_just_sent)
        {
            ImGui::SetNextWindowToFront();
        }
        ImGui::Begin("Console", &_is_open, ImGuiWindowFlags_NoFocusOnAppearing);

        _selected_message = MessageId{};
        MessageId msg_to_clear{};

        for (const auto& [id, msg] : _messages)
        {
            ImGui::PushID(&id);
            ImGui::BeginGroup();

            ImGui::TextColored(
                color(msg.message.severity),
                "[%s] [#%lu] [%s]",
                Cool::stringify(msg.timestamp).c_str(),
                msg.count,
                msg.message.category.c_str()
            );
            ImGui::SameLine();
            ImGui::Text("%s", msg.message.detailed_message.c_str());

            if (is_closable(msg))
            {
                ImGui::SameLine();
                if (ImGuiExtras::button_with_icon(
                        Icons::close_button().imgui_texture_id(),
                        ImVec4(0.9f, 0.9f, 0.9f, 1.f),
                        ImVec4(0.5f, 0.2f, 0.2f, 1.f),
                        11.f, 11.f
                    ))
                {
                    msg_to_clear = id; // We don't clear the message immediately because it would mess up our for-loop
                }
                ImGuiExtras::tooltip(("Clear this " + to_string(msg.message.severity)).c_str());
            }

            ImGui::EndGroup();
            ImGui::PopID();
            if (ImGui::IsItemHovered())
            {
                _selected_message = id;
            }
            if (_message_just_sent &&
                *_message_just_sent == id)
            {
                ImGui::SetScrollHereY(0.5f);
            }
        }
        clear(msg_to_clear);
        ImGui::End();
    }
    _message_just_sent.reset();
}

} // namespace Cool
