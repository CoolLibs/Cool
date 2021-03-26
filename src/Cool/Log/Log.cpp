#include "Log.h"

#include <Cool/String/String.h>

namespace Cool::Log {

std::string Release::_message = "";
bool Release::_open = false;
int Release::_messages_count = 0;
bool Release::_scroll_to_bottom = false;

void Release::Message(std::string_view message) {
	if (!_open)
		_message.clear();
	_message += "\n[";
	_message += String::ToString(_messages_count, 3) + "] ";
	_message += message;
	_open = true;
	_messages_count = (_messages_count + 1) % 1000;
	_scroll_to_bottom = true;
}

void Release::Show_Console() {
	if (_open) {
		ImGui::Begin("Console", &_open, ImGuiWindowFlags_NoFocusOnAppearing);
		ImGui::Text(_message.c_str());
		if (_scroll_to_bottom) {
			ImGui::SetScrollHereY(1.f);
			_scroll_to_bottom = false;
		}
		ImGui::End();
	}
}

void Release::ImGui_Toggle_Console() {
	ImGui::Checkbox("Console", &_open);
}

} // namespace Cool::Log