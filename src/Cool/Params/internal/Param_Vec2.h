#pragma once

#include "Param.h"

namespace Cool::Param {

class Vec2 : public Internal::Param<glm::vec2> {
public:
	Vec2(std::string_view name = "", glm::vec2 default_value = glm::vec2(0.f), glm::vec2 min_value = glm::vec2(0.f), glm::vec2 max_value = glm::vec2(1.f))
		: Param(name, default_value), _min_value(min_value), _max_value(max_value)
	{}

protected:
	bool ImGuiWidget() override {
		ImGui::PushID(this+34);
		ImGui::PushItemWidth(150);
		bool b = ImGui::SliderFloat("",             &_value.x, _min_value.x, _max_value.x);
		ImGui::PopID();
		ImGui::SameLine();
			b |= ImGui::SliderFloat(name().c_str(), &_value.y, _min_value.y, _max_value.y);
		ImGui::PopItemWidth();
		//ImGui::PushID(this);
		//if (ImGui::BeginPopupContextItem()) {
		//	ImGui::DragFloat("", &_min_value);
		//	ImGui::SameLine();
		//	ImGui::Text("to");
		//	ImGui::SameLine();
		//	ImGui::DragFloat(" ", &_max_value);
		//	ImGui::EndPopup();
		//}
		//ImGui::PopID();
		return b;
	}

private:
	glm::vec2 _min_value;
	glm::vec2 _max_value;
};

} // namespace Cool::Param