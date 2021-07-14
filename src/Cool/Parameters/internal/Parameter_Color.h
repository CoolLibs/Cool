#pragma once

#include "Parameter.h"

namespace Cool::Parameter {

class Color : public Internal::Parameter<glm::vec3> {
public:
    Color(std::string_view name = "", const glm::vec3& default_value = glm::vec3(0.f))
        : Parameter(name, default_value)
    {
    }

protected:
    bool imgui_widget() override
    {
        return ImGui::ColorEdit3(name().c_str(), glm::value_ptr(_value), ImGuiColorEditFlags_NoInputs);
    }
};

} // namespace Cool::Parameter