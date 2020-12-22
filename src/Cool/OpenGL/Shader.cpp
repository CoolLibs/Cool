#include "Shader.h"

#include <Cool/File/File.h>

namespace Cool {

#ifndef NDEBUG
#define ASSERT_SHADER_IS_BOUND GLint id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); assert(id == m_shaderId && "The shader must be bound before setting any uniform");
#else 
#define ASSERT_SHADER_IS_BOUND
#endif

static GLuint compileShader(GLenum type, const char* source) {
	GLCall(GLuint id = glCreateShader(type));
	GLCall(glShaderSource(id, 1, &source, nullptr));
	GLCall(glCompileShader(id));
#ifndef NDEBUG
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		Log::Error("Failed to compile {} {} :\n{}", (type == GL_FRAGMENT_SHADER ? "fragment" : "vertex"), "shader", message);
		glDeleteShader(id);
		return 0;
	}
#endif
	return id;
}

Shader::Shader(const char* vertexShaderFilepath, const char* fragmentShaderFilepath) {
	compile(vertexShaderFilepath, fragmentShaderFilepath);
}

Shader::~Shader() {
	GLCall(glDeleteProgram(m_shaderId));
}

void Shader::compile(const char* vertexShaderFilepath, const char* fragmentShaderFilepath) {
	if (m_shaderId != 0)
		GLCall(glDeleteProgram(m_shaderId));

	GLCall(m_shaderId = glCreateProgram());
	std::string vertexSrc, fragmentSrc;
	File::ToString(vertexShaderFilepath, &vertexSrc);
	File::ToString(fragmentShaderFilepath, &fragmentSrc);
	GLuint vs = compileShader(GL_VERTEX_SHADER, vertexSrc.c_str());
	GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentSrc.c_str());

	GLCall(glAttachShader(m_shaderId, vs));
	GLCall(glAttachShader(m_shaderId, fs));
	GLCall(glLinkProgram(m_shaderId));
	GLCall(glValidateProgram(m_shaderId));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));
}

void Shader::bind() {
	GLCall(glUseProgram(m_shaderId));
}

int Shader::getUniformLocation(const char* uniformName) {
	if (m_uniformLocationCache.find(uniformName) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[uniformName];
	}
	GLCall(GLint location = glGetUniformLocation(m_shaderId, uniformName));
	m_uniformLocationCache[uniformName] = location;
	return location;
}

void Shader::setUniform1i(const char* uniformName, int v) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniform1i(getUniformLocation(uniformName), v));
}
void Shader::setUniform1f(const char* uniformName, float v) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniform1f(getUniformLocation(uniformName), v));
}
void Shader::setUniform2f(const char* uniformName, const glm::vec2& v) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniform2f(getUniformLocation(uniformName), v.x, v.y));
}
void Shader::setUniform3f(const char* uniformName, const glm::vec3& v) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniform3f(getUniformLocation(uniformName), v.x, v.y, v.z));
}
void Shader::setUniform4f(const char* uniformName, const glm::vec4& v) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniform4f(getUniformLocation(uniformName), v.x, v.y, v.z, v.w));
}
void Shader::setUniformMat2f(const char* uniformName, const glm::mat2& mat) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniformMatrix2fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::setUniformMat3f(const char* uniformName, const glm::mat3& mat) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::setUniformMat4f(const char* uniformName, const glm::mat4& mat) {
	ASSERT_SHADER_IS_BOUND;
	GLCall(glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(mat)));
}

} // namespace Cool