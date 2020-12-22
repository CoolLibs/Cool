#pragma once

#include "internal/CameraState.h"
#include "Ray.h"

namespace Cool {

class Camera {
friend class CameraState_Idle;
friend class CameraState_Rotation;
friend class CameraState_Translation; 
public:
	Camera();
	~Camera() = default;

	inline const glm::mat4& transformMatrix() const { return m_transformMatrix; }
	inline const glm::mat4& viewMatrix() const { return m_viewMatrix; }
	inline const glm::mat4& projMatrix() const { return m_projMatrix; }
	glm::vec3 xAxis() const;
	glm::vec3 yAxis() const;
	glm::vec3 zAxis() const;
	glm::vec3 position() const;
	inline float focalLength() const { return m_focalLength; }

	/// <summary>
	/// Returns a Ray passing through the given pixel and starting at the camera position.
	/// Typically what you would want in order to check if a 3D object is under the mouse cursor by casting a ray : rayThroughPixel(Input::MouseInPixels())
	/// </summary>
	Ray rayThroughPixel(const glm::vec2& positionInPixels);

	inline void update() { m_state->update(); }
	inline void onWheelDown() { m_state->onWheelDown(); }
	inline void onWheelUp()   { m_state->onWheelUp();   }
	inline void onWheelScroll(float dl) { m_state->onWheelScroll(dl); }

private:
	void onTransformChanged();
	void computeProjectionMatrix();

	template <typename T, typename ...Args>
	inline void setState(Args&& ...args) {
		m_state = std::make_unique<T>(*this, std::forward<Args>(args)...);
	}

private:
	glm::vec3 m_lookAt = glm::vec3(0.0f);
	float m_distToLookAt = 3.0f;
	float m_angleGround = 0.0f;
	float m_angleUp = 0.0f;

	glm::mat4 m_transformMatrix;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projMatrix;

	float m_focalLength = 1.0f;

	std::unique_ptr<CameraState> m_state;
};

} // namespace Cool