#pragma once

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<glm::vec2> {
    friend auto operator<=>(const VariableMetadata<glm::vec2>&, const VariableMetadata<glm::vec2>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<glm::vec2>& var) -> bool
{
    return ImGui::DragFloat2(
        var.name.c_str(),
        glm::value_ptr(var.value)
    );
}

inline auto imgui_widget(VariableMetadata<glm::vec2>&) -> bool
{
    return false;
}

} // namespace Cool
