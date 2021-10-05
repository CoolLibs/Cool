#pragma once

#include <Cool/Geometry/Ray.h>
#include <Cool/Image/ImageSize.h>

namespace Cool {

class Camera {
public:
    Camera(glm::vec3 const& position = {15.f, 0.f, 0.f}, glm::vec3 const& look_at = glm::vec3{0.f});

    inline const glm::mat4& transform_matrix() const { return _transform_matrix; }
    inline glm::mat4        view_matrix() const { return glm::inverse(_transform_matrix); }
    inline const glm::mat4& projection_matrix() const { return _projection_matrix; }
    glm::vec3               right_axis() const;
    glm::vec3               up_axis() const;
    glm::vec3               front_axis() const;
    glm::vec3               position() const;

    inline void set_transform_matrix(const glm::mat4& transform_matrix) { _transform_matrix = transform_matrix; }
    inline void set_view_matrix(const glm::mat4& view_matrix) { _transform_matrix = glm::inverse(view_matrix); }
    inline void set_projection_matrix(const glm::mat4& projection_matrix) { _projection_matrix = projection_matrix; }

    /**
	 * @brief Translation expressed in world space
	 * 
	 * @param delta_position 
	 */
    void translate(glm::vec3 const& delta_position);

    /**
	 * @brief Rotation expressed in world space
	 * 
	 * @param center 
	 * @param angle 
	 * @param axis 
	 */
    void rotate_around(glm::vec3 const& center, float angle, glm::vec3 const& axis);
    void rotate(float angle, glm::vec3 const& axis);

    /**
	 * @brief Returns a Ray passing through the given pixel of an image and starting at the camera position. Typically what you would want in order to check if a 3D object is under the mouse cursor by casting a ray : ray_passing_through_pixel(Input::mouse_in_pixels())
	 * 
	 * @param position_in_pixels 
	 * @return 
	 */
    Ray ray_passing_through_pixel(const glm::vec2& position_in_pixels, ImageSize image_size) const;

private:
    glm::mat4 _transform_matrix  = glm::mat4{1.f};
    glm::mat4 _projection_matrix = glm::mat4{1.f};
};

} // namespace Cool