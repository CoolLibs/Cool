#pragma once

namespace Cool {

class ViewController_Orbital;
class Camera;

class IOrbitalState {
public:
	virtual void on_mouse_move  (ViewController_Orbital& controller, Camera& camera, glm::vec2 const& delta) {};
	virtual void on_wheel_scroll(ViewController_Orbital& controller, Camera& camera, float dl)               {};
	virtual void on_wheel_down  (ViewController_Orbital& controller, Camera& camera, int mods)               {};
	virtual void on_wheel_up    (ViewController_Orbital& controller, Camera& camera)                         {};
};

} // namespace Cool