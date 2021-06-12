#pragma once

#include <Cool/Geometry/Ray.h>

namespace Cool {

class Camera {
public:
	inline const glm::mat4& transform_matrix()  const { return _transform_matrix; }
	inline       glm::mat4  view_matrix()       const { return glm::inverse(_transform_matrix); }
	inline const glm::mat4& projection_matrix() const { return _projection_matrix; }
	glm::vec3 right_axis() const;
	glm::vec3 up_axis() const;
	glm::vec3 front_axis() const;
	glm::vec3 position() const;

	inline void set_transform_matrix (const glm::mat4& new_transform_matrix)  { _transform_matrix  = new_transform_matrix;	}
	inline void set_projection_matrix(const glm::mat4& new_projection_matrix) { _projection_matrix = new_projection_matrix;	}
	
	/**
	 * @brief Returns a Ray passing through the given pixel and starting at the camera position. Typically what you would want in order to check if a 3D object is under the mouse cursor by casting a ray : ray_passing_through_pixel(Input::mouse_in_pixels())
	 * 
	 * @param position_in_pixels 
	 * @return 
	 */
	Ray ray_passing_through_pixel(const glm::vec2& position_in_pixels);

private:
	glm::mat4 _transform_matrix;
	glm::mat4 _projection_matrix;
};

} // namespace Cool