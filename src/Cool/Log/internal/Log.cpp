#include "../Log.h"

#include <Cool/String/String.h>

namespace Cool::Log {

std::vector<Message> ToUser::_messages;
bool ToUser::_open = false;
int ToUser::_messages_count = 0;
bool ToUser::_scroll_to_bottom = false;

void ToUser::PushMessage(Message message) {
	_messages.push_back(message);
	_open = true;
	_scroll_to_bottom = true;
}

void ToUser::imgui_console_window() {
	if (_open) {
		ImGui::Begin("Console", &_open, ImGuiWindowFlags_NoFocusOnAppearing);
		for ( auto message : _messages) {
			ImGui::Text(message.body.c_str());
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