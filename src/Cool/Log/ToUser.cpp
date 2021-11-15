#include "ToUser.h"
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
        ImGui::Begin("Console", &_is_open, ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_MenuBar);
        ImGui::BeginMenuBar();
        if (ImGui::Button("Clear")) {
            _messages.clear();
        }
        ImGui::EndMenuBar();
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
            // const auto time = std::chrono::hh_mm_ss{message.timestamp.time_since_epoch()}; // hh_mm_ss isn't yet supported on GCC :(
            // ImGui::TextColored(color, "[%d'%lld\"] [%s]", time.minutes().count(), time.seconds().count(), message.category.c_str());
            const auto time = std::chrono::system_clock::to_time_t(message.timestamp);
#pragma warning(push)
#pragma warning(disable : 4996)
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
            const auto local_time = localtime(&time);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#pragma warning(pop)
            if (local_time) {
                ImGui::TextColored(color, "[%d'%d\"] [%s]", local_time->tm_min, local_time->tm_sec, message.category.c_str());
            }
            ImGui::SameLine();
            ImGui::Text("%s", message.body.c_str());
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