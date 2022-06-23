#pragma once

#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<bool> {
    friend auto operator<=>(const VariableMetadata<bool>&, const VariableMetadata<bool>&) = default;

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<bool>& var) -> bool
{
    return ImGui::Checkbox(var.name.c_str(), &var.value);
}

inline auto imgui_widget(VariableMetadata<bool>&) -> bool
{
    return false;
}

} // namespace Cool
