#pragma once

#include <glm/gtx/string_cast.hpp>
#include <type_traits>

namespace glm {
template<typename T>
auto operator<<(std::ostream& os, T const& t)
    // We need this enable_if to prevent "operator << is ambiguous"
    -> std::enable_if_t<
        std::is_same<T, glm::bvec2>::value ||
            std::is_same<T, glm::bvec3>::value ||
            std::is_same<T, glm::bvec4>::value ||
            std::is_same<T, glm::dmat2x2>::value ||
            std::is_same<T, glm::dmat2x3>::value ||
            std::is_same<T, glm::dmat2x4>::value ||
            std::is_same<T, glm::dmat3x2>::value ||
            std::is_same<T, glm::dmat3x3>::value ||
            std::is_same<T, glm::dmat3x4>::value ||
            std::is_same<T, glm::dmat4x2>::value ||
            std::is_same<T, glm::dmat4x3>::value ||
            std::is_same<T, glm::dmat4x4>::value ||
            std::is_same<T, glm::dvec2>::value ||
            std::is_same<T, glm::dvec3>::value ||
            std::is_same<T, glm::dvec4>::value ||
            std::is_same<T, glm::ivec2>::value ||
            std::is_same<T, glm::ivec3>::value ||
            std::is_same<T, glm::ivec4>::value ||
            std::is_same<T, glm::mat2x2>::value ||
            std::is_same<T, glm::mat2x3>::value ||
            std::is_same<T, glm::mat2x4>::value ||
            std::is_same<T, glm::mat3x2>::value ||
            std::is_same<T, glm::mat3x3>::value ||
            std::is_same<T, glm::mat3x4>::value ||
            std::is_same<T, glm::mat4x2>::value ||
            std::is_same<T, glm::mat4x3>::value ||
            std::is_same<T, glm::mat4x4>::value ||
            std::is_same<T, glm::uvec2>::value ||
            std::is_same<T, glm::uvec3>::value ||
            std::is_same<T, glm::uvec4>::value ||
            std::is_same<T, glm::vec2>::value ||
            std::is_same<T, glm::vec3>::value ||
            std::is_same<T, glm::vec4>::value,
        std::ostream&>
{
    os << glm::to_string(t);
    return os;
}
} // namespace glm