#include "../ParameterDynamicList.h"

namespace Cool {

void ParameterDynamicList::imgui(std::function<void()> on_value_change) {
	for (auto& param : _parameters) {
		std::visit([&](auto&& arg){ arg.imgui({}, on_value_change); }, param);
	}
}

void ParameterDynamicList::set_uniforms_in_shader(Shader& shader) {
	for (auto& param : _parameters) {
		std::visit([&](auto&& arg){ arg.set_uniform_in_shader(shader); }, param);
	}
}

size_t ParameterDynamicList::index_of(std::string_view parameter_name) {
	for (size_t i = 0; i < _parameters.size(); ++i) {
		const std::string& name = std::visit([&](auto&& arg){ return arg.name(); }, _parameters[i]);
		if (!parameter_name.compare(name)) {
			return i;
		}
	}
	return -1;
}

} // namespace Cool