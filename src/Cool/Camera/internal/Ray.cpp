#include "../Ray.h"

Cool::Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
	: m_origin(origin), m_direction(glm::normalize(direction))
{}