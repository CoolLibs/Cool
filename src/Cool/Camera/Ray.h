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
	Ray(const glm::vec3& origin, const glm::vec3& direction);
	~Ray() = default;

	/// <summary>
	/// Origin of the ray
	/// </summary>
	inline const glm::vec3& origin() { return m_origin; }
	/// <summary>
	/// Normalized direction of the ray
	/// </summary>
	inline const glm::vec3& direction() { return m_direction; }

private:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
};

} // namespace Cool