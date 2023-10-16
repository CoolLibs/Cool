#pragma once

#include "VariableRegistries.h"

namespace Cool {

// class ValueProvider {
// public:
//     ValueProvider(VariableRegistries& registries, float render_target_aspect_ratio, float height, float time, glm::mat3 const& camera2D)
//         : _variable_registries{registries}
//         , _render_target_aspect_ratio{render_target_aspect_ratio}
//         , _height{height}
//         , _time{time}
//         , _camera2D{camera2D}
//     {}

//     template<typename T>
//     auto operator()(Variable<T> const& var) const -> T
//     {
//         return var.value; // TODO(Variable) do we still need to go through the ValueProvider?
//     }

//     float operator()(const Input_AspectRatio&) const
//     {
//         return _render_target_aspect_ratio;
//     }

//     float operator()(const Input_Height&) const
//     {
//         return _height;
//     }

//     auto operator()(const Input_Time&) const -> float
//     {
//         return _time;
//     }

//     auto operator()(const Input_Camera2D&) const -> glm::mat3
//     {
//         return _camera2D;
//     }

//     auto variable_registries() -> VariableRegistries& { return _variable_registries; }

// private:
//     std::reference_wrapper<VariableRegistries> _variable_registries;
//     float                                      _render_target_aspect_ratio;
//     float                                      _height;
//     float                                      _time;
//     glm::mat3                                  _camera2D;
// };

} // namespace Cool
