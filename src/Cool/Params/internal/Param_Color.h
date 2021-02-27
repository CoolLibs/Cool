#pragma once

#include "IParam.h"

namespace Cool {
class Param_Color : public Param<glm::vec3> {
public:
	Param_Color(std::string_view name, const glm::vec3& default_value = glm::vec3(0.f));
	bool ImGui() override;
};

} // namespace Cool