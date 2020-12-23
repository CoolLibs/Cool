#include "CameraState_Rotation.h"
#include "CameraState_Idle.h"
#include "../Camera.h"
#include <Cool/App/Input.h>

namespace Cool {

CameraState_Rotation::CameraState_Rotation(Camera& camera)
	: CameraState(camera),
	  m_initialAngleGround(camera.m_angleGround),
	  m_initialAngleUp    (camera.m_angleUp),
	  m_initialMousePos(Input::MouseInCentimeters())
{}

void CameraState_Rotation::update() {
	glm::vec2 delta = (Input::MouseInCentimeters() - m_initialMousePos) * 0.3f;
	m_camera.m_angleGround = m_initialAngleGround + delta.x;
	m_camera.m_angleUp     = m_initialAngleUp     + delta.y;
	m_camera.onTransformChanged();
}

void CameraState_Rotation::onWheelUp() {
	m_camera.setState<CameraState_Idle>();
}

} // namespace Cool