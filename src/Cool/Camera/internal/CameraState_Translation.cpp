#include "CameraState_Translation.h"
#include "CameraState_Idle.h"
#include "../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

CameraState_Translation::CameraState_Translation(Camera& camera)
	: CameraState(camera),
	  m_initialLookAt(camera.m_lookAt),
	  m_initialMousePos(Input::MouseInScreenCoordinates())
{}

void CameraState_Translation::update() {
	glm::vec2 delta = (m_initialMousePos - Input::MouseInScreenCoordinates()) * 0.01f;
	m_camera.m_lookAt = m_initialLookAt + m_camera.xAxis() * delta.x - m_camera.yAxis() * delta.y;
	m_camera.onTransformChanged();
}

void CameraState_Translation::onWheelUp() {
	m_camera.setState<CameraState_Idle>();
}

} // namespace Cool