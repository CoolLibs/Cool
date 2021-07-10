#pragma once

namespace Cool {

class Camera;

class ProjectionController_Perspective {
public:
	void apply_to(Camera& camera) const;
	inline float focal_length() const { return _focal_length; }

private:
	float _focal_length = 1.f;
	float _near_plane   = 0.1f;
};

} // namespace Cool