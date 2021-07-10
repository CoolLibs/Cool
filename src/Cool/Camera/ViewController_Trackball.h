#pragma once

#include "internal/TrackballState/TrackballState.h"

namespace Cool {

class Camera;

class ViewController_Trackball {
public:
	void look_at_the_origin(Camera& camera);

	inline void on_mouse_move  (Camera& camera, glm::vec2 const& delta) { std::visit([&](auto&& state) { state.on_mouse_move  (*this, camera, delta); }, _state); }
	inline void on_wheel_down  (Camera& camera, int mods)               { std::visit([&](auto&& state) { state.on_wheel_down  (*this, camera, mods);  }, _state); }
	inline void on_wheel_up    (Camera& camera)                         { std::visit([&](auto&& state) { state.on_wheel_up    (*this, camera);        }, _state); }
	inline void on_wheel_scroll(Camera& camera, float dl)               { std::visit([&](auto&& state) { state.on_wheel_scroll(*this, camera, dl);    }, _state); }

private:
	friend class TrackballState_Idle;
	friend class TrackballState_Rotation;
	friend class TrackballState_Translation;

	inline void set_state(TrackballState const& state) { _state = state; }
	glm::vec3 get_look_at(Camera const& camera) const;

private:
	float _translation_speed = 0.01f;
	float _rotation_speed    = 0.01f;
	float _dist_to_look_at   = 3.0f;
	TrackballState _state = TrackballState_Idle{};
};

} // namespace Cool