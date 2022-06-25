#pragma once

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<glm::vec3> {
    friend auto operator<=>(const VariableMetadata<glm::vec3>&, const VariableMetadata<glm::vec3>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<glm::vec3>& var) -> bool
{
    return ImGui::DragFloat3(
        var.name.c_str(),
        glm::value_ptr(var.value)
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec3>&) -> bool
{
    return false;
}

} // namespace Cool
