#include "IParam.h"

#ifdef __COOL_OPENGL
#include <Cool/OpenGL/Shader.h>

namespace Cool::Internal {

template <>
void Param<int>::set_uniform_in_shader(Shader& shader) {
	shader.setUniform1i(name().c_str(), _value);
}

template <>
void Param<float>::set_uniform_in_shader(Shader& shader) {
	shader.setUniform1f(name().c_str(), _value);
}

template <>
void Param<glm::vec2>::set_uniform_in_shader(Shader& shader) {
	shader.setUniform2f(name().c_str(), _value);
}

template <>
void Param<glm::vec3>::set_uniform_in_shader(Shader& shader) {
	shader.setUniform3f(name().c_str(), _value);
}

template <>
void Param<glm::vec4>::set_uniform_in_shader(Shader& shader) {
	shader.setUniform4f(name().c_str(), _value);
}

} // namespace Cool
#endif