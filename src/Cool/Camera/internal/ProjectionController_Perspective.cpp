#include "../ProjectionController_Perspective.h"
#include <Cool/App/RenderState.h>
#include "../Camera.h"

namespace Cool {

void ProjectionController_Perspective::update_projection_matrix() {
	float near_plane = 0.1f;
	float fov = 2.f * atan(1.f / _focal_length); // in radians
	_camera.set_projection_matrix(
		glm::infinitePerspective(
			fov * 180.f / glm::pi<float>(),
			RenderState::Size().aspectRatio(),
			near_plane
		)
	);
}

} // namespace Cool