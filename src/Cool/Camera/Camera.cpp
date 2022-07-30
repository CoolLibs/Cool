#include "Camera.h"
#include <Cool/Constants/Constants.h>
#include <doctest/doctest.h>
#include <glm/gtc/matrix_access.hpp>

namespace Cool {

Camera::Camera(glm::vec3 const& position, glm::vec3 const& look_at)
    : _transform_matrix{glm::inverse(glm::lookAt(
          position,
          look_at,
          Constants::world_up
      ))}
{
}

glm::vec3 Camera::right_axis() const
{
    return glm::normalize(glm::column(_transform_matrix, 0));
}
glm::vec3 Camera::up_axis() const
{
    return glm::normalize(glm::column(_transform_matrix, 1));
}
glm::vec3 Camera::front_axis() const
{
    return -glm::normalize(glm::column(_transform_matrix, 2));
}
glm::vec3 Camera::position() const
{
    return glm::column(_transform_matrix, 3);
}

void Camera::translate(glm::vec3 const& delta_position)
{
    _transform_matrix = glm::translate(glm::mat4{1.f}, delta_position) * _transform_matrix;
}

void Camera::rotate_around(glm::vec3 const& center, float angle, glm::vec3 const& axis)
{
    translate(-center);
    rotate(angle, axis);
    translate(+center);
}

void Camera::rotate(float angle, glm::vec3 const& axis)
{
    _transform_matrix = glm::rotate(glm::mat4{1.f}, angle, axis) * _transform_matrix;
}

auto Camera::inverse_view_projection_matrix(float aspect_ratio) const -> glm::mat4
{
    return transform_matrix() * inverse_projection_matrix(aspect_ratio);
}

} // namespace Cool

#if DEBUG
TEST_CASE("Translating the camera only affects its position")
{
    // Given
    Cool::Camera camera;
    const auto   position   = camera.position();
    const auto   right_axis = camera.right_axis();
    const auto   front_axis = camera.front_axis();
    const auto   up_axis    = camera.up_axis();
    // When
    const auto translation = glm::vec3{10.3f, -5.1f, 8.f};
    camera.translate(translation);
    // Then
    CHECK(camera.position() == position + translation);
    CHECK(camera.right_axis() == right_axis);
    CHECK(camera.front_axis() == front_axis);
    CHECK(camera.up_axis() == up_axis);
}

TEST_CASE("Camera look_at constructor")
{
    // const auto position = glm::vec3{10.3f, -5.1f, 8.f};
    // Cool::Camera camera{position};
    // for (const auto x : position) {
    // 	INFO(position);
    // }
    // CHECK(camera.position() == position);
}
#endif