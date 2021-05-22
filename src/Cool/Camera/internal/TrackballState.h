#pragma once

namespace Cool {

class ViewController_Trackball;

class TrackballState {
public:
	TrackballState(ViewController_Trackball& controller)
		: _controller(controller)
	{}

	virtual void update() {};
	virtual void on_wheel_down(int mods) {};
	virtual void on_wheel_up() {};
	virtual void on_wheel_scroll(float dl) {};

protected:
	ViewController_Trackball& _controller;
};

} // namespace Cool