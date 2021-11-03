#include "ParameterU.h"

namespace Cool::ParameterU {

Parameter::Any make_param(const ParameterList& previous_parameters, const Parameter::AnyDesc& desc)
{
    const auto param_idx = index_of(desc, previous_parameters);
    if (param_idx.has_value()) {
        return (*previous_parameters)[*param_idx];
    }
    else {
        return make_any_parameter(desc);
    }
}

bool is_matching_desc_type(const Parameter::Any& param, const Parameter::AnyDesc& desc)
{
    return std::visit([&](auto&& param) { return is_matching_desc_type(param, desc); }, param);
}

std::optional<size_t> index_of(const Parameter::AnyDesc& description, const ParameterList& list)
{
    for (size_t i = 0; i < list->size(); ++i) {
        const std::string& name      = std::visit([&](auto&& param) { return param.name(); }, (*list)[i]);
        const std::string& desc_name = std::visit([&](auto&& desc) { return desc.name; }, description);
        if (desc_name == name && is_matching_desc_type((*list)[i], description)) {
            return i;
        }
    }
    return std::nullopt;
}

} // namespace Cool::ParameterU

TEST_CASE("[Cool::ParameterU] is_matching_desc_type()")
{
    CHECK(Cool::ParameterU::is_matching_desc_type(Cool::Parameter::Float{}, Cool::Parameter::FloatDesc{}));
    CHECK(Cool::ParameterU::is_matching_desc_type(Cool::Parameter::Color{}, Cool::Parameter::ColorDesc{}));
    CHECK(!Cool::ParameterU::is_matching_desc_type(Cool::Parameter::Float{}, Cool::Parameter::ColorDesc{}));
}