#pragma once

#include <Cool/Variables/Variables.h>
#include "Input.h"
#include "VariableId.h"
#include "VariableRegistries.h"

namespace Cool {

class InputFactory_Ref {
public:
    InputFactory_Ref(
        VariableRegistries&             variable_registries,
        const VariableId<Cool::Camera>& default_camera_id
    )
        : _variable_registries{variable_registries}
        , _default_camera_id{default_camera_id}
    {
    }

    template<typename T>
    auto make(
        const DirtyFlag&                  dirty_flag,
        std::string_view                  name,
        const std::optional<std::string>& description   = {},
        const T&                          default_value = {},
        const Cool::VariableMetadata<T>&  metadata      = {}
    )
        -> Input<T>
    {
        return Input{
            dirty_flag,
            name,
            description,
            _variable_registries.get().create(
                Cool::Variable<T>{std::string{name}, default_value, metadata}
            )};
    }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
    VariableId<Cool::Camera>                   _default_camera_id;
};

template<>
inline auto InputFactory_Ref::make(
    const DirtyFlag&                  dirty_flag,
    std::string_view                  name,
    const std::optional<std::string>& description,
    const Cool::Camera&,
    const Cool::VariableMetadata<Cool::Camera>&
)
    -> Input<Cool::Camera>
{
    return Input{
        dirty_flag,
        name,
        description,
        _default_camera_id};
}

} // namespace Cool
