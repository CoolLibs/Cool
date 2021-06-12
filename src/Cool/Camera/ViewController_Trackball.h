#pragma once

#include "internal/TrackballState.h"

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

	inline void update()                  { _state->update(); }
	inline void on_wheel_down(int mods)   { _state->on_wheel_down(mods); }
	inline void on_wheel_up()             { _state->on_wheel_up(); }
	inline void on_wheel_scroll(float dl) { _state->on_wheel_scroll(dl); }

private:
	friend class TrackballState_Idle;
	friend class TrackballState_Rotation;
	friend class TrackballState_Translation;
	template <typename T, typename ...Args>
	inline void set_state(Args&& ...args) {
		_state = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

	void update_transform_matrix();

private:
	Camera&   _camera;
	glm::vec3 _look_at = glm::vec3(0.0f);
	float     _dist_to_look_at = 3.0f;
	float     _angle_ground = 0.0f;
	float     _angle_up = 0.0f;
	std::unique_ptr<TrackballState> _state;
};

} // namespace Cool