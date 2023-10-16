#pragma once

#include <Cool/Variables/Variable.h>
#include <Cool/Variables/generated/all_variable_includes.inl>
#include <reg/cereal.hpp>
#include <reg/reg.hpp>

namespace Cool {

/// All the registries are owned by the top-level app, in order to simplify the changes through commands, the history, etc.
#include <Cool/Variables/generated/VariableRegistries.inl>

} // namespace Cool
