#pragma once
#include "Variables.h"

namespace Cool {

using AnyVariable = std::variant<
#include "generated/AnyVariable.inl"
    >;

} // namespace Cool
