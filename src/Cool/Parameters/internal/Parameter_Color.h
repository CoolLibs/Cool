#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

class Color : public Internal::Parameter<glm::vec3> {
public:
    Color(const ParameterDesc<glm::vec3>& desc = {})
        : Parameter{desc}
    {
    }

protected:
    bool imgui_widget() override
    {
        return ImGui::ColorEdit3(name().c_str(), glm::value_ptr(_value), ImGuiColorEditFlags_NoInputs);
    }
};

} // namespace Cool::Parameter