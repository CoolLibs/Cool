#pragma once

namespace Cool {

class ViewController_Trackball;
class Camera;

class ITrackballState {
public:
	virtual void on_mouse_move  (ViewController_Trackball& controller, Camera& camera, glm::vec2 const& delta) {};
	virtual void on_wheel_scroll(ViewController_Trackball& controller, Camera& camera, float dl)               {};
	virtual void on_wheel_down  (ViewController_Trackball& controller, Camera& camera, int mods)               {};
	virtual void on_wheel_up    (ViewController_Trackball& controller, Camera& camera)                         {};
};

} // namespace Cool