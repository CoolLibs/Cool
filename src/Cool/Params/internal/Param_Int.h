#pragma once

#include "IParam.h"

namespace Cool::Param {

class Int : public Internal::Param<int> {
public:
	Int(std::string_view name, int default_value = 0)
		: Param(name, default_value)
	{}

protected:
	bool ImGuiWidget() override {
		return ImGui::DragInt(name().c_str(), &_value);
	}
};

} // namespace Cool::Param