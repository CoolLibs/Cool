#pragma once

#include "IParam.h"

namespace Cool::Param {

class Float : public Internal::Param<float> {
public:
	Float(std::string_view name, float default_value = 0.f, float min_value = 0.f, float max_value = 1.f)
		: Param(name, default_value), _min_value(min_value), _max_value(max_value)
	{}

protected:
	bool ImGuiWidget() override {
		return ImGui::SliderFloat(name().c_str(), &_value, _min_value, _max_value);
	}

private:
	float _min_value;
	float _max_value;
};

} // namespace Cool::Param