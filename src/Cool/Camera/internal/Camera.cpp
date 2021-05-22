#include "../Camera.h"
#include <Cool/App/RenderState.h>
#include <glm/gtc/matrix_access.hpp>

namespace Cool {

glm::vec3 Camera::right_axis() const { return glm::normalize(glm::column(_transform_matrix, 0)); }
glm::vec3 Camera::up_axis()    const { return glm::normalize(glm::column(_transform_matrix, 1)); }
glm::vec3 Camera::front_axis() const { return glm::normalize(glm::column(_transform_matrix, 2)); }
glm::vec3 Camera::position()   const { return                glm::column(_transform_matrix, 3); }

Ray Camera::ray_passing_through_pixel(const glm::vec2& position_in_pixels) {
	glm::vec3 pos = position();
	glm::vec3 pixelPos = glm::unProject(glm::vec3(position_in_pixels, 0.0f), view_matrix(), projection_matrix(), glm::vec4(0.0f, 0.0f, RenderState::Size().width(), RenderState::Size().height())); // TODO use the size of the screen ? isn't it bugued out if render size > on_scree size ?
	glm::vec3 dir = pixelPos - pos;
	return Ray(pos, dir);
}

} // namespace Cool