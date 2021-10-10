#if defined(COOL_OPENGL)

#include "Shader.h"
#include "ShaderModule.h"

namespace Cool::OpenGL {

static void assert_shader_is_bound(GLuint id)
{
#if defined(DEBUG)
    GLint current_id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_id);
    assert(current_id == id && "The shader must be bound before setting any uniform.");
#endif
}

void validate_shader(GLuint id)
{
    GLDebug(glValidateProgram(id));
    GLint result;
    GLDebug(glGetProgramiv(id, GL_VALIDATE_STATUS, &result));
    if (result == GL_FALSE) {
        GLsizei length;
        GLDebug(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> error_message;
        error_message.reserve(length);
        GLDebug(glGetProgramInfoLog(id, length, nullptr, error_message.data()));
        throw std::invalid_argument(std::string{"Linking failed:\n"} + error_message.data());
    }
}

static void compile_shader(GLuint id, const std::vector<ShaderDescription>& shader_descriptions)
{
    for (const auto& shader_desc : shader_descriptions) {
        GLDebug(glAttachShader(id, ShaderModule{shader_desc}.id()));
    }
    GLDebug(glLinkProgram(id));
    validate_shader(id);
}

Shader::Shader(const std::vector<ShaderDescription>& shader_descriptions)
{
    compile_shader(_shader.id(), shader_descriptions);
}

void Shader::bind() const
{
    GLDebug(glUseProgram(_shader.id()));
}

GLint Shader::uniform_location(std::string_view uniform_name) const
{
    const auto name = std::string{uniform_name};
    const auto it   = _uniform_locations.find(name);
    if (it != _uniform_locations.end()) {
        return it->second;
    }
    else {
        GLDebug(GLint location = glGetUniformLocation(_shader.id(), name.c_str()));
        _uniform_locations[name] = location;
        return location;
    }
}

void Shader::set_uniform(std::string_view uniform_name, int v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform1i(uniform_location(uniform_name), v));
}
void Shader::set_uniform(std::string_view uniform_name, unsigned int v) const
{
    set_uniform(uniform_name, static_cast<int>(v));
}
void Shader::set_uniform(std::string_view uniform_name, bool v) const
{
    set_uniform(uniform_name, v ? 1 : 0);
}
void Shader::set_uniform(std::string_view uniform_name, float v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform1f(uniform_location(uniform_name), v));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec2& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform2f(uniform_location(uniform_name), v.x, v.y));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec3& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform3f(uniform_location(uniform_name), v.x, v.y, v.z));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::vec4& v) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniform4f(uniform_location(uniform_name), v.x, v.y, v.z, v.w));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat2& mat) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniformMatrix2fv(uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat3& mat) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniformMatrix3fv(uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(mat)));
}
void Shader::set_uniform(std::string_view uniform_name, const glm::mat4& mat) const
{
    assert_shader_is_bound(_shader.id());
    GLDebug(glUniformMatrix4fv(uniform_location(uniform_name), 1, GL_FALSE, glm::value_ptr(mat)));
}

} // namespace Cool::OpenGL

#endif