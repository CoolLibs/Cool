#include "../ProjectionController_Perspective.h"
#include "../Camera.h"
#include <Cool/App/RenderState.h>

namespace Cool {

void ProjectionController_Perspective::apply_to(Camera& camera) const {
	float near_plane = 0.1f;
	float fov = 2.f * atan(1.f / _focal_length); // in radians
	camera.set_projection_matrix(
		glm::infinitePerspective(
			glm::degrees(fov),
			RenderState::Size().aspectRatio(),
			near_plane
		)
	);
}

} // namespace Cool