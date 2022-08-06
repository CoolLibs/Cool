#include "MessageConsole.h"
#include <Cool/Constants/Constants.h>
#include <Cool/Icons/Icons.h>
#include <Cool/ImGui/ImGuiExtras.h>
#include <stringify/stringify.hpp>

namespace Cool {

void MessageConsole::send(MessageId& id, const Message& message)
{
    if (!_messages.contains(id))
    {
        id = MessageId{
            _messages,
            internal::MessageWithMetadata{
                .message = message,
            },
        };
    }
    else
    {
        _messages.with_mutable_ref(id, [&](internal::MessageWithMetadata& data) {
            data.message = message;
            data.count++;
            data.timestamp           = std::chrono::system_clock::now();
            data.monotonic_timestamp = std::chrono::steady_clock::now();
        });
    }

    on_message_sent(id);
}

void MessageConsole::send(const Message& message)
{
    const auto id = _messages.create(internal::MessageWithMetadata{
        .message               = message,
        .forced_to_be_closable = true,
    }
    );

    on_message_sent(id);
}

void MessageConsole::on_message_sent(const internal::RawMessageId& id)
{
    _is_open           = true;
    _message_just_sent = id;
    refresh_counts_per_severity();
}

void MessageConsole::clear(const MessageId& id)
{
    clear(id.get());
}

void MessageConsole::clear(const internal::RawMessageId& id)
{
    if (_messages.contains(id))
    {
        _messages.destroy(id);

        if (_messages.is_empty())
        {
            close_window();
        }
    }
    refresh_counts_per_severity();
}

void MessageConsole::close_window()
{
    _is_open          = false;
    _selected_message = {};
}

auto MessageConsole::should_highlight(const MessageId& id) -> bool
{
    return !id.get().underlying_uuid().is_nil() &&
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
        Log::Debug::error("MessageConsole::color", "Unknown enum value");
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
        Log::Debug::error("MessageConsole::to_string", "Unknown enum value");
        return "";
    }
}

static auto is_closable(const internal::MessageWithMetadata& msg) -> bool
{
    return msg.forced_to_be_closable ||
           msg.message.severity != MessageSeverity::Error;
}

void MessageConsole::clear_all()
{
    {
        std::unique_lock lock{_messages.mutex()};
        std::erase_if(_messages.underlying_container().underlying_container(), [](auto&& pair) {
            return is_closable(pair.second);
        });
    }

    if (_messages.is_empty())
    {
        close_window();
    }
    refresh_counts_per_severity();
}

void MessageConsole::show_number_of_messages_of_given_severity(MessageSeverity severity)
{
    const auto count = _counts_per_severity.get(severity);
    if (count > 0)
    {
        ImGui::SameLine();
        ImGui::TextColored(
            color(severity),
            "%zu %s%s",
            count,
            to_string(severity).c_str(),
            count > 1 ? "s" : ""
        );
    }
}

void MessageConsole::imgui_window()
{
    if (_is_open)
    {
        if (!_message_just_sent.underlying_uuid().is_nil())
        {
            ImGui::SetNextWindowToFront();
        }
        ImGui::Begin(_name, nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

        // Menu bar
        ImGuiExtras::background(
            [&]() { imgui_menu_bar(); },
            ImVec4{0.3f, 0.3f, 0.3f, 0.5f}
        );
        ImGui::Separator();

        // All the messages
        ImGui::BeginChild(
            "##Messages", ImVec2(0.f, 0.f), false,
            ImGuiWindowFlags_AlwaysUseWindowPadding
        );
        imgui_show_all_messages();
        ImGui::EndChild();

        ImGui::End();
    }
    _message_just_sent = {};
}

void MessageConsole::imgui_menu_bar()
{
    if (ImGui::Button("Clear All"))
    {
        clear_all();
    }
    show_number_of_messages_of_given_severity(MessageSeverity::Error);
    show_number_of_messages_of_given_severity(MessageSeverity::Warning);
    show_number_of_messages_of_given_severity(MessageSeverity::Info);
}

void MessageConsole::imgui_show_all_messages()
{
    _selected_message = {};                // Clear the selected message. And let the following loop set it again if necessary.
    internal::RawMessageId msg_to_clear{}; // Let the loop store a `msg_to_clear`. We don't clear the message immediately because it would mess up our for-loop and cause a deadlock with the `lock`.
    {
        std::shared_lock lock{_messages.mutex()};
        for (const auto& id_and_message : _messages)
        {
            const auto& id     = id_and_message.first;
            const auto& msg    = id_and_message.second;
            const auto  widget = [&]() {
                ImGui::PushID(&id);
                ImGui::BeginGroup();

                ImGui::TextColored(
                     color(msg.message.severity),
                     "[%s] [#%u] [%s]",
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
                        msg_to_clear = id;
                    }
                    ImGuiExtras::tooltip(("Clear this " + to_string(msg.message.severity)).c_str());
                }

                ImGui::EndGroup();
                ImGui::PopID();

                if (ImGui::IsItemHovered())
                {
                    _selected_message = id;
                }
                if (_message_just_sent == id)
                {
                    ImGui::SetScrollHereY(0.5f);
                }
            };

            // Draw highlight of recent messages
            const auto dt = std::chrono::duration<float>{
                std::chrono::steady_clock::now() - msg.monotonic_timestamp};
            static constexpr float highlight_duration = 0.5f;
            if (dt.count() < highlight_duration)
            {
                ImGuiExtras::highlight(
                    widget,
                    1.f - dt.count() / highlight_duration
                );
            }
            else
            {
                widget();
            }
        }
    }
    clear(msg_to_clear); // Must clear after the `lock` is gone, otherwise we will deadlock.
}

void MessageConsole::refresh_counts_per_severity()
{
    _counts_per_severity.reset_to_zero();
    std::shared_lock lock{_messages.mutex()};
    for (const auto& id_and_message : _messages)
    {
        _counts_per_severity.increment(id_and_message.second.message.severity);
    }
}

MessageConsole::MessagesCountPerSeverity::MessagesCountPerSeverity()
{
    reset_to_zero();
}

void MessageConsole::MessagesCountPerSeverity::increment(MessageSeverity severity)
{
    _counts_per_severity[static_cast<size_t>(severity)]++;
}

void MessageConsole::MessagesCountPerSeverity::reset_to_zero()
{
    _counts_per_severity.fill(0);
}

auto MessageConsole::MessagesCountPerSeverity::get(MessageSeverity severity) const -> size_t
{
    return _counts_per_severity[static_cast<size_t>(severity)];
}

} // namespace Cool
