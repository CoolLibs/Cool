#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/Direction2D.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Direction2D> {
    friend auto operator<=>(const VariableMetadata<Direction2D>&, const VariableMetadata<Direction2D>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<Direction2D>& var) -> bool
{
    return ImGuiExtras::angle_wheel(
        var.name.c_str(),
        &var.value.angle.value.value
    );
}

inline auto imgui_widget(VariableMetadata<Direction2D>&) -> bool
{
    return false;
}

} // namespace Cool
