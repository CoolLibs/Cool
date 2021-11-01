#pragma once

#include "Parameter.h"

namespace Cool {

Parameter::AnyDesc parse_one_parameter_desc(std::string_view source);

template<ParameterDesc Desc>
Desc parse(std::string_view source);

} // namespace Cool

#include "Parsing.tpp"