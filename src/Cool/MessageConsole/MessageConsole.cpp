#include "MessageConsole.h"
#include <Cool/Constants/Constants.h>
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <imgui/imgui_internal.h>
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
    _message_just_sent = true;
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
            ImGui::SetNextWindowFocus();
        }
        ImGui::Begin("Console", &_is_open, ImGuiWindowFlags_NoFocusOnAppearing);

        _selected_message = MessageId{};
        MessageId msg_to_clear{};

        for (const auto& [id, msg] : _messages)
        {
            ImGui::PushID(&id);
            ImGui::BeginGroup();

            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            // Draw gradients
            // (note that those are currently exacerbating our sRGB/Linear issues)
            // Calling ImGui::GetColorU32() multiplies the given colors by the current Style Alpha, but you may pass the IM_COL32() directly as well..
            ImGui::Text("Gradients");
            ImVec2 gradient_size = ImVec2(100.f, ImGui::GetFrameHeight());
            {
                ImVec2 p0    = ImGui::GetCursorScreenPos();
                ImVec2 p1    = ImVec2(p0.x + gradient_size.x, p0.y + gradient_size.y);
                ImU32  col_a = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
                ImU32  col_b = ImGui::GetColorU32(IM_COL32(255, 255, 255, 255));
                draw_list->AddRectFilled(p0, p1, col_b);
                // ImGui::InvisibleButton("##gradient1", gradient_size);
            }

            ImGui::TextColored(
                color(msg.message.severity),
                "[%s] [#%lld] [%s]",
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
        }
        clear(msg_to_clear);

        if (_message_just_sent)
        {
            ImGui::SetScrollHereY(1.f);
        }
        ImGui::End();
    }
    _message_just_sent = false;
}

} // namespace Cool
