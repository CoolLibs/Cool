#include "../Camera.h"
#include "CameraState_Idle.h"
#include <Cool/App/RenderState.h>

#include "Sign.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>

namespace Cool {

Camera::Camera() {
	setState<CameraState_Idle>();
	onTransformChanged();
	computeProjectionMatrix();
	RenderState::SubscribeToSizeChanges([this]() { computeProjectionMatrix(); });
}

void Camera::onTransformChanged() {
	glm::vec3 posRelToLookAt = glm::vec3(
		cos(m_angleUp) * cos(m_angleGround),
		sin(m_angleUp),
		cos(m_angleUp) * sin(m_angleGround)
	) * m_distToLookAt;
	m_viewMatrix = glm::lookAt(posRelToLookAt + m_lookAt, m_lookAt, glm::vec3(0.0f, Math::sign(cos(m_angleUp)), 0.0f));
	m_transformMatrix = glm::inverse(m_viewMatrix);
}

void Camera::computeProjectionMatrix() {
	float nearPlane = 0.1f;
	float fov = 2.f * atan(1.f / m_focalLength); // in radians
	m_projMatrix = glm::infinitePerspective(fov * 180.f / glm::pi<float>(), RenderState::Size().aspectRatio(), nearPlane);
}

glm::vec3 Camera::xAxis() const { return glm::normalize(glm::column(m_transformMatrix, 0)); }
glm::vec3 Camera::yAxis() const { return glm::normalize(glm::column(m_transformMatrix, 1)); }
glm::vec3 Camera::zAxis() const { return glm::normalize(glm::column(m_transformMatrix, 2)); }
glm::vec3 Camera::position() const { return glm::column(m_transformMatrix, 3); }

Ray Camera::rayThroughPixel(const glm::vec2& positionInPixels) {
	glm::vec3 pos = position();
	glm::vec3 pixelPos = glm::unProject(glm::vec3(positionInPixels, 0.0f), viewMatrix(), projMatrix(), glm::vec4(0.0f, 0.0f, RenderState::Size().width(), RenderState::Size().height()));
	glm::vec3 dir = pixelPos - pos;
	return Ray(pos, dir);
}

} // namespace Cool