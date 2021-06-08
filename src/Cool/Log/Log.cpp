#include "Log.h"

#include <Cool/String/String.h>

namespace Cool::Log {

std::string ToUser::_message = "";
bool ToUser::_open = false;
int ToUser::_messages_count = 0;
bool ToUser::_scroll_to_bottom = false;

void ToUser::Message(std::string_view message) {
#ifdef __COOL_IMGUI_LIB
	if (!_open)
		_message.clear();
	_message += "\n[";
	_message += String::to_string(_messages_count, 3) + "] ";
	_message += message;
	_open = true;
	_messages_count = (_messages_count + 1) % 1000;
	_scroll_to_bottom = true;
#else
	Log::Error("You need to include the ImGui library (Which is packaged with the App module). If you are not using App, then include ImGui and define __COOL_IMGUI_LIB (this can be done in CMake with add_compile_definitions(__COOL_IMGUI_LIB))");
#endif
}

void ToUser::Show_Console() {
#ifdef __COOL_IMGUI_LIB
	if (_open) {
		ImGui::Begin("Console", &_open, ImGuiWindowFlags_NoFocusOnAppearing);
		ImGui::Text(_message.c_str());
		if (_scroll_to_bottom) {
			ImGui::SetScrollHereY(1.f);
			_scroll_to_bottom = false;
		}
		ImGui::End();
	}
#else
	Log::Error("You need to include the ImGui library (Which is packaged with the App module). If you are not using App, then include ImGui and define __COOL_IMGUI_LIB (this can be done in CMake with add_compile_definitions(__COOL_IMGUI_LIB))");
#endif
}

void ToUser::ImGui_Toggle_Console() {
#ifdef __COOL_IMGUI_LIB
	ImGui::Checkbox("Console", &_open);
#else
	Log::Error("You need to include the ImGui library (Which is packaged with the App module). If you are not using App, then include ImGui and define __COOL_IMGUI_LIB (this can be done in CMake with add_compile_definitions(__COOL_IMGUI_LIB))");
#endif
}

} // namespace Cool::Log