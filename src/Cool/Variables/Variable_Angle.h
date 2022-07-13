#pragma once

#include <Cool/StrongTypes/Angle.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Angle> {
    friend auto operator<=>(const VariableMetadata<Angle>&, const VariableMetadata<Angle>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<Angle>& var) -> bool
{
    return Cool::imgui_angle_widget(
        var.name,
        var.value
    );
}

inline auto imgui_widget(VariableMetadata<Angle>&) -> bool
{
    return false;
}

} // namespace Cool
