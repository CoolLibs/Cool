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
    auto hue         = var.value.get();
    bool has_changed = hue_widget(var.name, hue);
    var.value.set(hue);
    return has_changed;
}

inline auto imgui_widget(VariableMetadata<Hue>&) -> bool
{
    return false;
}

} // namespace Cool
