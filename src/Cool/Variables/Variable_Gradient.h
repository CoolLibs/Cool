#pragma once

#include <Cool/ImGui/ImGuiExtras.h>
#include <Cool/StrongTypes/Gradient.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Gradient> {
    bool is_hdr = true;

    friend auto operator<=>(const VariableMetadata<Gradient>&, const VariableMetadata<Gradient>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<Gradient>& var) -> bool
{
    return gradient_widget(
        var.value
    );
}

inline auto imgui_widget(VariableMetadata<Gradient>&) -> bool
{
    return false;
}

} // namespace Cool
