#include "../Camera.h"
#include <Cool/App/RenderState.h>
#include <glm/gtc/matrix_access.hpp>

namespace Cool {

glm::vec3 Camera::right_axis() const { return  glm::normalize(glm::column(_transform_matrix, 0)); }
glm::vec3 Camera::up_axis()    const { return  glm::normalize(glm::column(_transform_matrix, 1)); }
glm::vec3 Camera::front_axis() const { return -glm::normalize(glm::column(_transform_matrix, 2)); }
glm::vec3 Camera::position()   const { return                 glm::column(_transform_matrix, 3) ; }

void Camera::translate(glm::vec3 const& delta_position) {
	_transform_matrix = glm::translate(glm::mat4{1.f}, delta_position) * _transform_matrix;
}

void Camera::rotate_around(glm::vec3 const& center, float angle, glm::vec3 const& axis) {
	translate(-center);
	rotate(angle, axis);
	translate(+center);
}

void Camera::rotate(float angle, glm::vec3 const& axis) {
	_transform_matrix = glm::rotate(glm::mat4{1.f}, angle, axis) * _transform_matrix;
}

Ray Camera::ray_passing_through_pixel(const glm::vec2& position_in_pixels) {
	glm::vec3 pos = position();
	glm::vec3 pixel_pos = glm::unProject(glm::vec3(position_in_pixels, 0.0f), view_matrix(), projection_matrix(), glm::vec4(0.0f, 0.0f, RenderState::Size().width(), RenderState::Size().height())); // TODO use the size of the screen ? isn't it bugued out if render size > on_scree size ?
	glm::vec3 dir = pixel_pos - pos;
	return Ray(pos, dir);
}

} // namespace Cool