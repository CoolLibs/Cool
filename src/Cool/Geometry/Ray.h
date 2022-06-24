#pragma once

namespace Cool {

/**
 * @brief Holds an origin and a normalized direction
 *
 */
class Ray {
public:
    /**
     * @brief
     *
     * @param origin
     * @param direction Doesn't need to be normalized because the constructor will do it anyways
     */
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : _origin(origin), _direction(glm::normalize(direction))
    {
    }

    /**
     * @brief
     *
     * @return The origin of the ray
     */
    inline const glm::vec3& origin() { return _origin; }

    /**
     * @brief
     *
     * @return The normalized direction of the ray
     */
    inline const glm::vec3& direction() { return _direction; }

private:
    glm::vec3 _origin;
    glm::vec3 _direction;
};

} // namespace Cool