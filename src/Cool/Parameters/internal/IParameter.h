#pragma once

#include <Cool/History/Action.h>

namespace Cool::Internal {

/**
 * @brief Interface that all parameters should implement
 * 
 */
class IParameter {
public:
	virtual ~IParameter() = default;

	virtual const std::string& name() const = 0;
	virtual bool ImGui(Action on_edit_ended, std::function<void()> on_value_change = []() {}) = 0;
	virtual void set_uniform_in_shader(Shader& shader) = 0;

protected:
	virtual bool ImGuiWidget() = 0;
};

} // namespace Cool::Internal