#include "ParameterDynamicList.h"

namespace Cool {

void ParameterDynamicList::imgui(std::function<void()> on_value_change)
{
    for (auto& param : _parameters) {
        std::visit([&](auto&& param) { param.imgui({}, on_value_change); }, param);
    }
}

size_t ParameterDynamicList::index_of(std::string_view parameter_name)
{
    for (size_t i = 0; i < _parameters.size(); ++i) {
        const std::string& name = std::visit([&](auto&& param) { return param.name(); }, _parameters[i]);
        if (parameter_name == name) {
            return i;
        }
    }
    return -1;
}

} // namespace Cool