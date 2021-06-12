#include "../ParameterDynamicList.h"

namespace Cool {

void ParameterDynamicList::imgui(std::function<void()> on_value_change) {
	for (auto& param : _parameters) {
		std::visit([&](auto&& arg){ arg.imgui({}, on_value_change); }, param);
	}
}

} // namespace Cool