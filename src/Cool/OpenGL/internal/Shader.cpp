#include "../Shader.h"

#include <Cool/File/File.h>

namespace Cool {

#ifndef NDEBUG
#define ASSERT_SHADER_IS_BOUND GLint id; glGetIntegerv(GL_CURRENT_PROGRAM, &id); assert(id == m_programID && "The shader must be bound before setting any uniform");
#else 
#define ASSERT_SHADER_IS_BOUND
#endif

ShaderCode::ShaderCode(ShaderType type, const char* filePath)
	: type(type)
{
	File::ToString(filePath, &sourceCode);
}

ShaderCode ShaderCode::FromCode(ShaderType type, const std::string& sourceCode) {
	ShaderCode shader;
	shader.type = type;
	shader.sourceCode = sourceCode;
	return shader;
}

Shader::Shader(const std::vector<ShaderCode>& shaderCodes) {
	createProgram(shaderCodes);
}

Shader::Shader(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) 
	: Shader({ShaderCode(ShaderType::Vertex, vertexShaderFilePath), ShaderCode(ShaderType::Fragment, fragmentShaderFilePath)})
{}

Shader::Shader(Shader&& o) noexcept
	: m_programID(o.m_programID)
{
	o.m_programID = 0;
}

void Shader::operator=(Shader&& o) {
	m_programID = o.m_programID;
	o.m_programID = 0;
}

Shader::~Shader() {
	glDeleteProgram(m_programID);
}

void Shader::createProgram(const std::vector<ShaderCode>& shaderCodes) {
	// Create program
	if (m_programID != 0)
		GLCall(glDeleteProgram(m_programID));
	GLCall(m_programID = glCreateProgram());
	// Compile shaders
	std::vector<GLuint> shaderIDs;
	shaderIDs.reserve(shaderCodes.size());
	for (const auto& shaderCode : shaderCodes) {
		shaderIDs.push_back(CreateShader(shaderCode));
		GLCall(glAttachShader(m_programID, shaderIDs.back()));
	}
	// Link
	GLCall(glLinkProgram(m_programID));
	GLCall(glValidateProgram(m_programID));
	// Delete shaders
	for (auto shaderID : shaderIDs) {
		glDeleteShader(shaderID);
	}
}

void Shader::createProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {
	createProgram({ ShaderCode(ShaderType::Vertex, vertexShaderFilePath), ShaderCode(ShaderType::Fragment, fragmentShaderFilePath) });
}

GLuint Shader::CreateShader(const ShaderCode& shaderCode) {
	// Get shader type
	GLenum shaderType;
	switch (shaderCode.type) {
	case ShaderType::Vertex:
		shaderType = GL_VERTEX_SHADER;
		break;
	case ShaderType::Fragment:
		shaderType = GL_FRAGMENT_SHADER;
		break;
	case ShaderType::Compute:
		shaderType = GL_COMPUTE_SHADER;
		break;
	default:
		Log::Error("Unknown shader type !");
	}
	// Create
	GLCall(GLuint shaderID = glCreateShader(shaderType));
	// Compile
	const char* src = shaderCode.sourceCode.c_str();
	GLCall(glShaderSource(shaderID, 1, &src, nullptr));
	GLCall(glCompileShader(shaderID));
	// Debug
#ifndef NDEBUG
	int result;
	GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(shaderID, length, &length, message));
		Log::Error("Shader compilation failed :\n{}", message);
		GLCall(glDeleteShader(shaderID));
	}
#endif
	return shaderID;
}

void Shader::bind() {
	GLCall(glUseProgram(m_programID));
}

GLint Shader::getUniformLocation(const char* uniformName) {
	if (m_uniformLocationCache.find(uniformName) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[uniformName];
	}
	GLCall(GLint location = glGetUniformLocation(m_programID, uniformName));
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