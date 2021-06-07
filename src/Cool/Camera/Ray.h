#pragma once

namespace Cool {

/// <summary>
/// Holds an origin and a normalized direction
/// </summary>
class Ray {
public:
	/// <summary>
	/// direction doesn't need to be normalized because the constructor will do it anyways
	/// </summary>
	Ray(const glm::vec3& origin, const glm::vec3& direction)
		: _origin(origin), _direction(glm::normalize(direction))
	{}

	/// <summary>
	/// Origin of the ray
	/// </summary>
	inline const glm::vec3& origin() { return _origin; }
	/// <summary>
	/// Normalized direction of the ray
	/// </summary>
	inline const glm::vec3& direction() { return _direction; }

private:
	glm::vec3 _origin;
	glm::vec3 _direction;
};

} // namespace Cool