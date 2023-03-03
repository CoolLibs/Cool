#pragma once

#include <Cool/Variables/Variable.h>
#include <reg/reg.hpp>

namespace Cool {

template<typename T>
using VariableId = reg::Id<Cool::Variable<T>>;
template<typename T>
using SharedVariableId = reg::SharedId<Cool::Variable<T>>;

} // namespace Cool