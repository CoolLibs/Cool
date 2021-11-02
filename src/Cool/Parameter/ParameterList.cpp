#include "ParameterList.h"

namespace Cool {

ParameterList::ParameterList(const std::vector<Parameter::AnyDesc>& descriptions)
{
    _parameters.reserve(descriptions.size());
    for (const auto& desc : descriptions) {
        _parameters.push_back(make_any_parameter(desc));
    }
}

void ParameterList::imgui(std::function<void()> on_value_change)
{
    for (auto& param : _parameters) {
        std::visit([&](auto&& param) { param.imgui({}, on_value_change); }, param);
    }
}

} // namespace Cool