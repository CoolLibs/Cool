#include "../Log.h"
#include <iomanip>

#include <Cool/String/String.h>

namespace Cool::Log {

std::vector<Message> ToUser::_messages;
bool ToUser::_open = false;
int ToUser::_messages_count = 0;
bool ToUser::_scroll_to_bottom = false;

void ToUser::PushMessage(Message message) {
	if(_messages.empty() || _messages.back().category != message.category) {
		_messages.push_back(message);
	}
	else {
		_messages.back() = message;
	}
	_open = true;
	_scroll_to_bottom = true;
}

void ToUser::imgui_console_window() {
	if (_open) {
		ImGui::Begin("Console", &_open, ImGuiWindowFlags_NoFocusOnAppearing);
		for ( auto message : _messages) {
			const ImVec4 color = [&]() {
				switch(message.severity){
				case Message::Severity::Info :
					return ImVec4{1,1,1,1};
				case Message::Severity::Warn :
					return ImVec4{1,1,0,1};
				case Message::Severity::Error :
					return ImVec4{1,0,0,1};
				default:
					Log::error("[ToUser::imgui_console_window] Unknown enum value");
					return ImVec4{0,0,0,0};
				}
			}();
			// Get minutes and seconds
			const std::time_t time = std::chrono::system_clock::to_time_t(message.timestamp);
			std::stringstream min_sec;
			min_sec <<  std::put_time(std::localtime(&time), "%M'%S\"");
			//
			ImGui::TextColored(color, "[%s] [%s] %s", min_sec.str().c_str(), message.category.c_str(), message.body.c_str());
		}
		if (_scroll_to_bottom) {
			ImGui::SetScrollHereY(1.f);
			_scroll_to_bottom = false;
		}
		ImGui::End();
	}
}

void ToUser::imgui_toggle_console() {
	ImGui::Checkbox("Console", &_open);
}

} // namespace Cool::Log