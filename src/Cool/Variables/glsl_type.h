#pragma once

namespace Cool {

template<typename T>
auto glsl_type() -> std::string;

#include "generated/glsl_type.inl"

} // namespace Cool