#include "Param_Color.h"

namespace Cool::Param {

Color::Color(std::string_view name, const glm::vec3& default_value)
	: Param(name, default_value)
{}

bool Color::ImGui() {
	bool b = ImGui::ColorEdit3(name().c_str(), glm::value_ptr(_value), ImGuiColorEditFlags_NoInputs);
	push_change_in_history_if_edit_ended();
	return b;
}

} // namespace Cool