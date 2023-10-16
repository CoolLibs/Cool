#pragma once

#include "VariableId.h"

namespace Cool {

class SetVariableDirty_Ref {
public:
    SetVariableDirty_Ref(SetDirty_Ref set_dirty)
        : _inputs{all_inputs}
        , _set_dirty{set_dirty}
    {
    }

    template<typename T>
    void operator()(const VariableId<T>& variable_id)
    {
        for (const auto& input : _inputs)
        {
            const auto* input_of_right_type = std::get_if<std::reference_wrapper<const Cool::Input<T>>>(&input);
            if (input_of_right_type)
            {
                input_of_right_type->get().set_dirty_if_depends_on(variable_id, _set_dirty);
            }
        }
    }

private:
    AllInputRefsToConst _inputs;
    SetDirty_Ref        _set_dirty;
};

} // namespace Cool
