#pragma once

#include "IParam.h"

namespace Cool::Param {

class Int : public Internal::ParamForPrimitiveType<int> {
public:
	Int(std::string_view name, int default_value = 0, int min_value = 0, int max_value = 20)
		: ParamForPrimitiveType(name, default_value), _min_value(min_value), _max_value(max_value)
	{}

protected:
	bool ImGuiWidget() override {
		bool b = ImGui::SliderInt(name().c_str(), &_value, _min_value, _max_value);
		ImGui::PushID(this);
		if (ImGui::BeginPopupContextItem()) {
			ImGui::DragInt("", &_min_value);
			ImGui::SameLine();
			ImGui::Text("to");
			ImGui::SameLine();
			ImGui::DragInt(" ", &_max_value);
			ImGui::EndPopup();
		}
		ImGui::PopID();
		return b;
	}

private:
	int _min_value;
	int _max_value;
};

} // namespace Cool::Param