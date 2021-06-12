#pragma once

namespace Cool {

class Camera;

class ViewController_Freefly {
public:
	/**
	 * @brief 
	 * 
	 * @param camera The camera to control
	 */
	ViewController_Freefly(Camera& camera);

	//inline void update() { _state->update(); }
	//inline void on_wheel_down(int mods) { _state->on_wheel_down(mods); }
	//inline void on_wheel_up() { _state->on_wheel_up(); }
	//inline void on_wheel_scroll(float dl) { _state->on_wheel_scroll(dl); }

private:
	void update_transform_matrix();

private:
	Camera& _camera;
	float   _angle_ground = 0.f;
	float   _angle_up = 0.f;
};

} // namespace Cool