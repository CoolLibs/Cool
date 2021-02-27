#pragma once

#include "IParam.h"

namespace Cool {
class Param_Color : public IParam {
public:
	Param_Color(std::string_view name, const glm::vec3& default_value = glm::vec3(0.f));
	bool ImGui() override;
	inline glm::vec3& operator* () { return  _value; }
	inline glm::vec3* operator->() { return &_value; }

private:
	glm::vec3 _value;
};

} // namespace Cool