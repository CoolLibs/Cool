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

std::optional<size_t> ParameterList::index_of(std::string_view parameter_name)
{
    for (size_t i = 0; i < _parameters.size(); ++i) {
        const std::string& name = std::visit([&](auto&& param) { return param.name(); }, _parameters[i]);
        if (parameter_name == name) {
            return i;
        }
    }
    return std::nullopt;
}

} // namespace Cool