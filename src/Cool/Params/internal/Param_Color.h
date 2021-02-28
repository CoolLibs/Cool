#pragma once

#include "IParam.h"

namespace Cool::Param {

class Color : public Internal::Param<glm::vec3> {
public:
	Color(std::string_view name, const glm::vec3& default_value = glm::vec3(0.f))
		: Param(name, default_value)
	{}

protected:
	bool ImGuiWidget() override {
		return ImGui::ColorEdit3(name().c_str(), glm::value_ptr(_value), ImGuiColorEditFlags_NoInputs);
	}
};

} // namespace Cool::Param