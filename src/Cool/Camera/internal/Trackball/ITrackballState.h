#pragma once

namespace Cool {

class ViewController_Trackball;

class ITrackballState {
public:
	virtual void update         (ViewController_Trackball& controller)           {};
	virtual void on_wheel_scroll(ViewController_Trackball& controller, float dl) {};
	virtual void on_wheel_down  (ViewController_Trackball& controller, int mods) {};
	virtual void on_wheel_up    (ViewController_Trackball& controller)           {};
};

} // namespace Cool