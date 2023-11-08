#pragma once

#include <Cool/Variables/Variables.h>
#include <utility>
#include "Input.h"
#include "InputDefinition.h"
#include "VariableId.h"
#include "VariableRegistries.h"

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
    auto make(InputDefinition<T> const& def, DirtyFlag dirty_flag, DirtyFlag secondary_dirty_flag = {}) const -> Input<T>
    {
        return Input{
            dirty_flag,
            secondary_dirty_flag,
            def.name,
            def.description,
            _variable_registries.get().create_shared(
                Cool::Variable<T>{{def.name, def.default_value, def.metadata}}
            ),
            def.desired_color_space,
        };
    }

    auto make(AnyInputDefinition const&, DirtyFlag dirty_flag, DirtyFlag secondary_dirty_flag = {}) const -> AnyInput;

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
    SharedVariableId<Cool::Camera>             _default_camera_id;
};

template<>
inline auto InputFactory_Ref::make(InputDefinition<Camera> const& def, DirtyFlag dirty_flag, DirtyFlag secondary_dirty_flag) const -> Input<Camera>
{
    return Input{
        dirty_flag,
        secondary_dirty_flag,
        def.name,
        def.description,
        _default_camera_id,
        def.desired_color_space,
    };
}

} // namespace Cool
