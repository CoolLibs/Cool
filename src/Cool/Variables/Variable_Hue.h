#pragma once

#include <Cool/StrongTypes/Hue.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Hue> {
    friend auto operator<=>(const VariableMetadata<Hue>&, const VariableMetadata<Hue>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<Hue>& var) -> bool
{
    return imgui_widget(var.name, var.value);
}

inline auto imgui_widget(VariableMetadata<Hue>&) -> bool
{
    return false;
}

} // namespace Cool
