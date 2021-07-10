#include "../../Camera.h"
#include "../../ViewController_Trackball.h"
#include "TrackballState_Idle.h"

#include <Cool/Utils/sign.h>
#include <glm/gtc/matrix_transform.hpp>

namespace Cool {

ViewController_Trackball::ViewController_Trackball(Camera& camera)
	: _camera(camera), _state(TrackballState_Idle{})
{
	update_transform_matrix();
}

void ViewController_Trackball::update_transform_matrix() {
	glm::vec3 position = _look_at + glm::vec3(
		cos(_angle_up) * cos(_angle_ground),
		sin(_angle_up),
		cos(_angle_up) * sin(_angle_ground)
	) * _dist_to_look_at;

	_camera.set_transform_matrix(
		glm::inverse(
			glm::lookAt(
				position,
				_look_at,
				glm::vec3(0.f, Math::sign(cos(_angle_up)), 0.f) /* World Up vector */
			)
		)
	);
}

} // namespace Cool