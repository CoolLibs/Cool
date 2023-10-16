#pragma once
#include "AnyVariable.h"

namespace Cool {

/// List of values contained by a preset.
using Settings = std::vector<Cool::AnyVariable>; // TODO(Variable) Should not store variables, but just a value and metadata (and also a name or id to identify the variables and make sure they haven't changed). When applying a preset, set the default value and default metadata to those of the preset.

} // namespace Cool