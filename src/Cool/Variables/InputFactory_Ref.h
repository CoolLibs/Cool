#pragma once

#include <utility>
#include "AnySharedVariableId.h"
#include "VariableDefinition.h"
#include "VariableId.h"
#include "VariableRegistries.h"
#include "Variables.h"

namespace Cool {

class InputFactory_Ref {
public:
    InputFactory_Ref(
        VariableRegistries&            variable_registries,
        SharedVariableId<Cool::Camera> default_camera_id
    )
        : _variable_registries{variable_registries}
        , _default_camera_id{std::move(default_camera_id)}
    {
    }

    template<typename T>
    auto make(VariableDefinition<T> const& def, DirtyFlag const& dirty_flag) const -> SharedVariableId<T>
    {
        return _variable_registries.get().create_shared(Variable<T>{
            .name                = def.name,
            .description         = def.description,
            .value               = def.default_value,
            .metadata            = def.metadata,
            .dirty_flag          = dirty_flag,
            .desired_color_space = def.desired_color_space,
        });
    }

    auto make(AnyVariableDefinition const&, DirtyFlag const&) const -> AnySharedVariableId;

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
    SharedVariableId<Cool::Camera>             _default_camera_id;
};

// template<>
// inline auto InputFactory_Ref::make(VariableDefinition<Camera> const& def, DirtyFlag const& dirty_flag) const -> SharedVariableId<Camera>
// {
//     return Input{
//         dirty_flag,
//         def.name,
//         def.description,
//         _default_camera_id,
//         def.desired_color_space,
//     };
// }

} // namespace Cool
