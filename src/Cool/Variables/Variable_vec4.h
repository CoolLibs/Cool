#pragma once

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<glm::vec4> {
    friend auto operator<=>(const VariableMetadata<glm::vec4>&, const VariableMetadata<glm::vec4>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<glm::vec4>& var) -> bool
{
    return ImGui::DragFloat4(
        var.name.c_str(),
        glm::value_ptr(var.value)
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec4>&) -> bool
{
    return false;
}

} // namespace Cool
