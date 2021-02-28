#pragma once

#include "IParam.h"

namespace Cool::Param {

class Color : public Internal::Param<glm::vec3> {
public:
	Color(std::string_view name, const glm::vec3& default_value = glm::vec3(0.f));

protected:
	bool ImGuiWidget() override;
};

} // namespace Cool::Param