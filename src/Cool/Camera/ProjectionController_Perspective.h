#pragma once

namespace Cool {

class Camera;

class ProjectionController_Perspective {
public:
	/**
	 * @brief 
	 * 
	 * @param The camera to control 
	 */
	ProjectionController_Perspective(Camera& camera)
		: _camera(camera)
	{
		update_projection_matrix();
	}

	inline float focal_length() const { return _focal_length; }

private:
	void update_projection_matrix();

private:
	float   _focal_length = 1.f;
	Camera& _camera;
};

} // namespace Cool