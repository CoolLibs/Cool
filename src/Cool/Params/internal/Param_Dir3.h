#pragma once

#include "Param.h"
#include <Cool/ImGuiExtras/ImGuiExtras.h>

namespace Cool::Internal {

class Dir3Angles : public Internal::Param<glm::vec2> {
public:
	Dir3Angles(std::string_view name, float default_angle_ground = 0.f, float default_angle_up = 0.f)
		: Param(name, glm::vec2(default_angle_ground, default_angle_up))
	{}

	bool ImGuiWidget() override {
		return ImGui::direction_3d(name().c_str(), &_value.x, &_value.y);
	}
};

} // namespace Cool::Internal

namespace Cool::Param {

class Dir3 : public Cool::Internal::IParam {
public:
	Dir3(std::string_view name = "", float default_angle_ground = 0.f, float default_angle_up = 0.f)
		: _angles(name, default_angle_ground, default_angle_up)
	{}

	inline const glm::vec3 operator* () const { return compute_direction(); }

	glm::vec3 compute_direction() const {
		return glm::vec3(
			 cos(_angles->y) * cos(_angles->x),
			-cos(_angles->y) * sin(_angles->x),
			-sin(_angles->y)
		);
	}

	bool ImGui(Action on_edit_ended, std::function<void()> on_value_change = []() {}) override {
		return _angles.ImGui(on_edit_ended, on_value_change);
	}

	void set_uniform_in_shader(Shader& shader) override {
		shader.set_uniform(name(), operator*());
	}

	inline const std::string& name() const override { return _angles.name(); }

protected:
	bool ImGuiWidget() override {
		return _angles.ImGuiWidget();
	}

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(_angles);
	}

private:
	Internal::Dir3Angles _angles;
};

} // namespace Cool::Param