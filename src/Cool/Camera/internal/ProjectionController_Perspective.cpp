#include "../ProjectionController_Perspective.h"
#include "../Camera.h"
#include <Cool/App/RenderState.h>

namespace Cool {

void ProjectionController_Perspective::apply_to(Camera& camera) const {
	const float fov = 2.f * atan(1.f / _focal_length); // in radians
	camera.set_projection_matrix(
		glm::infinitePerspective(
			glm::degrees(fov),
			RenderState::Size().aspectRatio(),
			_near_plane
		)
	);
}

} // namespace Cool