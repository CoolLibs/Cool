#pragma once

#include "Cool/Dependencies/Input.h"
#include "Input.h"
#include "VariableRegistries.h"

namespace Cool {

class InputProvider_Ref {
public:
    InputProvider_Ref(VariableRegistries& registries, float render_target_aspect_ratio, float height, float time, glm::mat3 const& camera2D)
        : _variable_registries{registries}
        , _render_target_aspect_ratio{render_target_aspect_ratio}
        , _height{height}
        , _time{time}
        , _camera2D{camera2D}
    {}

    template<typename T>
    auto operator()(Input<T> const& input) const -> T
    {
        { // Try the current variable
            auto const maybe_variable = _variable_registries.get().get(input._current_variable_id);
            if (maybe_variable)
            {
                return maybe_variable->value();
            }
        }
        { // Try the default variable
            auto const maybe_variable = _variable_registries.get().get(input._default_variable_id.raw());
            if (maybe_variable)
            {
                return maybe_variable->value();
            }
        }
        // Default, this should not happen
        Cool::Log::Debug::error(
            "InputProvider_Ref::operator()",
            "An Input has an invalid default variable!"
        );
        return T{};
    }

    float operator()(const Input_AspectRatio&) const
    {
        return _render_target_aspect_ratio;
    }

    float operator()(const Input_Height&) const
    {
        return _height;
    }

    auto operator()(const Input_Time&) const -> float
    {
        return _time;
    }

    auto operator()(const Input_Camera2D&) const -> glm::mat3
    {
        return _camera2D;
    }

    auto variable_registries() -> VariableRegistries& { return _variable_registries; }

private:
    std::reference_wrapper<VariableRegistries> _variable_registries;
    float                                      _render_target_aspect_ratio;
    float                                      _height;
    float                                      _time;
    glm::mat3                                  _camera2D;
};

} // namespace Cool
