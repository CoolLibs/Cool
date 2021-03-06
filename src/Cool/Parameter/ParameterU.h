#pragma once

#include "ParameterList.h"

namespace Cool::ParameterU {

Parameter::Any make_param(const ParameterList& previous_parameters, const Parameter::AnyDesc& desc);

Cool::ParameterList update_parameters(std::vector<Cool::Parameter::AnyDesc> description, Cool::ParameterList param);

Cool::ParameterList update_parameters(std::vector<Cool::Parameter::Any> description, Cool::ParameterList param);

std::optional<size_t> index_of(const Parameter::AnyDesc& description, const ParameterList& list);

bool is_matching_desc_type(const Parameter::Any& param, const Parameter::AnyDesc& desc);

template<ParameterType Param>
bool is_matching_desc_type(const Param& /*param*/, const Parameter::AnyDesc& desc)
{
    return std::holds_alternative<typename Param::Description>(desc);
}

} // namespace Cool::ParameterU