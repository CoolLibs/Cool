#pragma once
#include <Cool/Dependencies/Input.h>
#include <Cool/Variables/generated/all_types_includes.inl>

namespace Cool {

#include <Cool/Variables/generated/AnyInput.inl>
#include <Cool/Variables/generated/AnyInputRef.inl>          // TODO(Variables) Remove ?
#include <Cool/Variables/generated/AnyInputRefToConst.inl>   // TODO(Variables) Remove ?
using AllInputRefsToConst = std::vector<AnyInputRefToConst>; // TODO(Variables) Remove ?

} // namespace Cool