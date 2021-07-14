#include "../ToUser.h"
#include <Cool/Constants/Constants.h>
#include <Cool/String/String.h>
#include <iomanip>
#include <sstream>

namespace Cool::Log {

std::vector<Message> ToUser::_messages;
bool                 ToUser::_is_open          = false;
bool                 ToUser::_scroll_to_bottom = false;

void ToUser::add_message(Message message)
{
    // Merge messages of the same category together
    if (!_messages.empty() && _messages.back().category == message.category) {
        _messages.back() = message;
    }
    // or add the message to the list
    else {
        _messages.push_back(message);
    }
    // Update console
    _is_open          = true;
    _scroll_to_bottom = true;
}

void ToUser::imgui_console_window()
{
    if (!_is_open) {
        _messages.clear();
    }
    else {
        ImGui::Begin("Console", &_is_open, ImGuiWindowFlags_NoFocusOnAppearing);
        for (auto const& message : _messages) {
            const ImVec4 color = [&]() {
                switch (message.severity) {
                case Message::Severity::Info:
                    return Constants::imvec4_green;
                case Message::Severity::Warn:
                    return Constants::imvec4_yellow;
                case Message::Severity::Error:
                    return Constants::imvec4_red;
                default:
                    Log::error("[ToUser::imgui_console_window] Unknown enum value");
                    return ImVec4{0, 0, 0, 0};
                }
            }();
            // Get minutes and seconds
            const std::time_t time = std::chrono::system_clock::to_time_t(message.timestamp);
            std::stringstream min_sec;
            min_sec << std::put_time(std::localtime(&time), "%M'%S\"");
            //
            ImGui::TextColored(color, "[%s] [%s]", min_sec.str().c_str(), message.category.c_str());
            ImGui::SameLine();
            ImGui::Text(message.body.c_str());
        }
        if (_scroll_to_bottom) {
            ImGui::SetScrollHereY(1.f);
            _scroll_to_bottom = false;
        }
        ImGui::End();
    }
}

void ToUser::imgui_toggle_console()
{
    ImGui::Checkbox("Console", &_is_open);
}

} // namespace Cool::Log