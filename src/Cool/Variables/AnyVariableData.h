#pragma once
#include "Variables.h"

namespace Cool {

// TODO(Variables) Use a polymorphic approach that is less taxing in compile time ? Like type-erasure ?
using AnyVariableData = std::variant<
#include "generated/AnyVariableData.inl"
    >;

} // namespace Cool
