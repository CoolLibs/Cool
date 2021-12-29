#pragma once

#include <Cool/Constants/Constants.h>

namespace Cool {

class Camera {
public:
    Camera(glm::vec3 const& position = Constants::default_camera_position, glm::vec3 const& look_at = glm::vec3{0.f});

    glm::mat4 transform_matrix() const { return _transform_matrix; }
    glm::mat4 view_matrix() const { return glm::inverse(_transform_matrix); }
    glm::mat4 projection_matrix() const { return _projection_matrix; }
    glm::vec3 right_axis() const;
    glm::vec3 up_axis() const;
    glm::vec3 front_axis() const;
    glm::vec3 position() const;

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

private:
    glm::mat4 _transform_matrix  = glm::mat4{1.f};
    glm::mat4 _projection_matrix = glm::mat4{1.f};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Transform", _transform_matrix),
                cereal::make_nvp("Projection", _projection_matrix));
    }
};

} // namespace Cool