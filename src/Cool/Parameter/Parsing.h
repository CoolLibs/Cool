#pragma once

#include "Parameter.h"

namespace Cool {

std::vector<Parameter::AnyDesc>                      parse_all_parameter_desc(std::string_view source);
std::optional<std::pair<Parameter::AnyDesc, size_t>> parse_one_parameter_desc(std::string_view source);
template<ParameterDesc Desc>
Desc parse(std::string_view source);

} // namespace Cool

#include "Parsing.tpp"