#include "Param_Color.h"

namespace Cool {

Param_Color::Param_Color(std::string_view name, const glm::vec3& default_value) 
	: IParam(name, default_value)
{}

bool Param_Color::ImGui() {
	return ImGui::ColorEdit3(name().c_str(), glm::value_ptr(_value), ImGuiColorEditFlags_NoInputs);
}

} // namespace Cool