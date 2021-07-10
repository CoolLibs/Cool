#pragma once

#include "internal/Trackball/TrackballState.h"

namespace Cool {

class Camera;

class ViewController_Trackball {
public:
	/**
	 * @brief 
	 * 
	 * @param camera The camera to control
	 */
	ViewController_Trackball(Camera& camera);

	inline void update()                  { std::visit([&](auto&& arg) { arg.update         (*this);       }, _state); }
	inline void on_wheel_down(int mods)   { std::visit([&](auto&& arg) { arg.on_wheel_down  (*this, mods); }, _state); }
	inline void on_wheel_up()             { std::visit([&](auto&& arg) { arg.on_wheel_up    (*this);       }, _state); }
	inline void on_wheel_scroll(float dl) { std::visit([&](auto&& arg) { arg.on_wheel_scroll(*this, dl);   }, _state); }

private:
	friend class TrackballState_Idle;
	friend class TrackballState_Rotation;
	friend class TrackballState_Translation;

	inline void set_state(const TrackballState& state) {
		_state = state;
	}
	void update_transform_matrix();

private:
	Camera&   _camera;
	glm::vec3 _look_at         = glm::vec3(0.0f);
	float     _dist_to_look_at = 3.0f;
	float     _angle_ground    = 0.0f;
	float     _angle_up        = 0.0f;
	TrackballState _state;
};

} // namespace Cool