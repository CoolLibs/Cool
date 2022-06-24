#pragma once

#include <Cool/Camera/Camera.h>
#include "Variable.h"

namespace Cool {

template<>
struct VariableMetadata<Cool::Camera> {
    friend auto operator<=>(const VariableMetadata<Cool::Camera>&, const VariableMetadata<Cool::Camera>&) = default;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive&)
    {
    }
};

inline auto imgui_widget(Variable<Cool::Camera>&) -> bool
{
    return false;
}

inline auto imgui_widget(VariableMetadata<Cool::Camera>&) -> bool
{
    return false;
}

} // namespace Cool
