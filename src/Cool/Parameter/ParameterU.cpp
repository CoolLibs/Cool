#include "ParameterU.h"

namespace Cool::ParameterU {

bool is_matching_type(const Parameter::Any& param, const Parameter::AnyDesc& desc)
{
    return std::visit([&](auto&& param) { return is_matching_type2(param, desc); }, param);
}

std::optional<size_t> index_of(const Parameter::AnyDesc& description, const ParameterList& list)
{
    for (size_t i = 0; i < list->size(); ++i) {
        const std::string& name      = std::visit([&](auto&& param) { return param.name(); }, (*list)[i]);
        const std::string& desc_name = std::visit([&](auto&& desc) { return desc.name; }, description);
        if (desc_name == name && is_matching_type((*list)[i], description)) {
            return i;
        }
    }
    return std::nullopt;
}

} // namespace Cool::ParameterU

TEST_CASE("[Cool::ParameterU] is_matching_type()")
{
    CHECK(Cool::ParameterU::is_matching_type(Cool::Parameter::Float{}, Cool::Parameter::FloatDesc{}));
    CHECK(Cool::ParameterU::is_matching_type(Cool::Parameter::Color{}, Cool::Parameter::ColorDesc{}));
    CHECK(!Cool::ParameterU::is_matching_type(Cool::Parameter::Float{}, Cool::Parameter::ColorDesc{}));
}